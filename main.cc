#include <iostream>
#include <ftw.h>
#include <getopt.h>

#include <identify.h>
#include <datastore.h>

Identify 		ident;
Datastore		store;
std::string base = "/vol/beta";

std::string get_destination(file_meta m)
{
	std::string path = base;
	// set filename = <date>_<original_name>.<lowercase_ext>
	// set type = video | image | document
	// set path = <base>/<type>/year/month/<filename>
	return path;
}

int process(const char* path, const struct stat * sb, int typeflag, struct FTW* ftwbuf)
{
  auto result = ident.process(path);
	std::cout << "DBG: " << result << std::endl;
  if (result.creation_date.empty()) {
    // Fall back to file date
    char buff[20];
    // Use ugly date since it is changed later
    strftime(buff, 20, "%Y:%m:%d %H:%M:%S", localtime(&sb->st_mtime));
    result.creation_date = buff;
  }
	if (!store.set( result.path, result.type, result.creation_date, 
                  result.width, result.height, result.duration, get_destination(result))) {
		std::cerr << "Datastore error: " << store.get_error() << std::endl;
    return 1;
	}
	return 0;
}

int print_version(std::string progname)
{
	std::cerr << progname << ", version " << VERSION << ", built " << __DATE__ << " " << __TIME__ << std::endl;
	return 0;
}

int print_usage(std::string progname)
{
	std::cerr << "Usage: " << progname << " [-hv] [-b DIR] [-t DIR] [-d DB]" << std::endl;
	std::cerr << "Find it, identify it, date it and in the end Organize it\n";
	std::cerr << '\n';
	std::cerr << "  -t, --traverse=DIR		traverse directory DIR\n";
	std::cerr << "  -b, --base=DIR				basepath of new location\n";
	std::cerr << "  -d, --dump=DB					dump contents of sqlite database DB\n";
	std::cerr << '\n';
	std::cerr << "  -h, --help			display this help and exit\n";
	std::cerr << "  -v, --version		output version information and exit\n";
	std::cerr << '\n';
	std::cerr << progname << " will traverse the directory tree starting at DIR and\n";
	std::cerr << "attempt to identify each file it encounters. The result is stored in\n";
	std::cerr << "an sqlite database.\n";
	std::cerr << "It can also dump the findings from a previous run, stored in DB\n";
  return 0;
}

int main(int argc, char** argv)
{
	const struct option long_opts[] =
	{	
		{"help",			no_argument, 0, 'h'},
		{"version",		no_argument, 0, 'v'},
		{"traverse",	required_argument, 0, 't'},
		{"dump",			required_argument, 0, 'd'}
	};
	std::string dir = ".";
	std::string db_path;
	int opt=0;
	int option_index=0;
	while ((opt = getopt_long(argc, argv, "d:ht:vV", long_opts, &option_index)) != -1)
	{
		switch (opt)
		{
			case 'd':	db_path = optarg; break;
			case 't': dir = optarg; break;
			case 'v':
			case 'V': return print_version(argv[0]); break;
			case 'h':
			default: return print_usage(argv[0]); break;
		}
	}
	
	if (!db_path.empty()) {
		std::cerr << "The dump feature is not yet implemented" << std::endl;
		return 1;
	}

  // First init identify, without it nothing will work
  if (!ident.init()) {
    std::cerr << "FIDO failed: " << ident.get_error() << std::endl;  
		return 1;
  }
	
	// Init datastore, without it nothing matters anyway
	if (!store.open()) {
		std::cerr << "FIDO failed: " << store.get_error() << std::endl;
		return 1;
	}

	std::cout << "FIDO will traverse directories, starting at " << dir << std::endl;

	int res = nftw(dir.c_str(),process,1024,FTW_PHYS);

  std::vector<std::string> data;
  store.dump(data);
  for (auto row : data) {
    std::cout << row << std::endl;  
  }

	return res;
}

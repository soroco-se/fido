#include <iostream>
#include <ftw.h>

#include <identify.h>

Identify ident;

int process(const char* path, const struct stat * sb, int typeflag, struct FTW* ftwbuf)
{
	// Check filetype, using magic!
//	auto magic_info = magic_file(magic_cookie, path);
//	std::cout << path << " " << magic_info << std::endl;
  std::cout << ident.process(path);
	
	return 0;
}

int main(int argc, char** argv)
{
	std::string dir = ".";
	if (argc == 2)
	  dir = argv[1];

  // First init identify, without it nothing will work
  if (!ident.init()) {
    std::cerr << "FIDO failed: " << ident.get_error() << std::endl;  
  }

	std::cout << "FIDO will traverse directories, starting at " << dir << std::endl;

	int res = nftw(dir.c_str(),process,1024,FTW_PHYS);


	return res;
}

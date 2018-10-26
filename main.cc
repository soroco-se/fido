#include <iostream>
#include <ftw.h>
#include <magic.h>

magic_t magic_cookie;


int process(const char* path, const struct stat * sb, int typeflag, struct FTW* ftwbuf)
{
	// Check filetype, using magic!
	auto magic_info = magic_file(magic_cookie, path);
	std::cout << path << " " << magic_info << std::endl;
	
	return 0;
}

int main(int argc, char** argv)
{
	std::string dir = ".";
	if (argc == 2)
	  dir = argv[1];

	std::cout << "FIDO will traverse directories, starting at " << dir << std::endl;

	// Setup magic
	magic_cookie = magic_open(MAGIC_NONE);
	if (magic_cookie == NULL) {
		std::cerr << "Unable to load magic" << std::endl;
		return 1;
	}
	if (magic_load(magic_cookie, NULL) != 0) {
		std::cerr << "Could not load magic database: " << magic_error(magic_cookie) << std::endl;
		magic_close(magic_cookie);
		return 1;
	}

	int res = nftw(dir.c_str(),process,1024,FTW_PHYS);

	magic_close(magic_cookie);

	return res;
}

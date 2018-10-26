#include <identify.h>
#include <sstream>

Identify::Identify()
{
}

Identify::~Identify()
{
	magic_close(magic_cookie);
}
 
bool Identify::init()
{
  bool ret=true;
	// Setup magic
	magic_cookie = magic_open(MAGIC_NONE);
	if (magic_cookie == NULL) {
		error = "Unable to load magic";
		return false;
	}
	if (magic_load(magic_cookie, NULL) != 0) {
		error = "Could not load magic database: ";
    error.append( magic_error(magic_cookie) );
		magic_close(magic_cookie);
		return false;
	}
  return ret;
}

std::string Identify::process(std::string path)
{
  std::stringstream ss;
	auto magic_info = magic_file(magic_cookie, path.c_str());
	ss << path << " " << magic_info << std::endl;
  return ss.str();
}

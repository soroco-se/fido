#ifndef IDENTIFY_H
#define IDENTIFY_H

#include <magic.h>
#include <string>

class Identify
{
  public:
    Identify();
    ~Identify();
    bool init();
    std::string get_error() { return error; }

    std::string process(std::string path);
    

  private:
    magic_t magic_cookie;
    std::string error;
};

#endif

#ifndef IDENTIFY_H
#define IDENTIFY_H

#include <magic.h>
#include <extractor.h>
#include <iostream>
#include <string>

struct file_meta
{
  file_meta() {}
  file_meta(std::string p) : path(p)  {}
  std::string path;           // Path to the file investigated
  std::string ext;            // File extension
  std::string magic_info;     // Information from libmagic
  std::string creation_date;  // From exif or similar metadata
  std::string size;           // From exif or similar metadata
  std::string extraction_plugin;
  std::string other;
};

inline std::ostream& operator<<(std::ostream& os, const struct file_meta& m)
{
  os << m.path << " " << m.ext << " " << m.magic_info << " " << m.creation_date << " " 
     << m.size << " " << m.extraction_plugin << " " << m.other;
  return os;
}

class Identify
{
  public:
    Identify();
    ~Identify();
    bool init();
    std::string get_error() { return error; }

    file_meta process(std::string path);
    int meta(std::string plugin_name, enum EXTRACTOR_MetaType type, std::string data); 

  private:
    magic_t magic_cookie;

    struct EXTRACTOR_PluginList* extractor_all;
    struct EXTRACTOR_PluginList* extractor_img;
    struct EXTRACTOR_PluginList* extractor_vid;

    file_meta current;
    std::string error;
};

#endif

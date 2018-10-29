#ifndef IDENTIFY_H
#define IDENTIFY_H

#include <magic.h>
#include <extractor.h>
#include <iostream>
#include <string>
#include <stdexcept>

enum FileType {
  OTHER = 0,
  IMAGE = 1,
  VIDEO = 2,
  PDF   = 3,
  DOC   = 4,
  TEXT  = 5,
	NOFILE = 10
};

inline std::string to_string(FileType ft) {
	switch(ft) {
		case FileType::NOFILE: return "nofile"; break;				
		case FileType::OTHER:  return "other"; break;				
		case FileType::IMAGE:  return "image"; break;				
		case FileType::VIDEO:  return "video"; break;				
		case FileType::PDF:  	 return "pdf";		break;				
		case FileType::DOC:  	 return "doc"; 	break;				
		case FileType::TEXT: 	 return "text";	break;				
	}				
}

struct file_meta
{
  file_meta() {}
  file_meta(std::string p) : path(p),width(0),height(0),duration(0.0),file_type(FileType::NOFILE),all_set(false)  {}
	// Parameters to be exported
  std::string path;           ///< Path to the file investigated
	std::string type;						///< Type of file, serialization of FileType
  std::string creation_date;  ///< From exif or similar metadata
  int 				width;					///< Set for images and video
  int 				height;					///< Set for images and video
  double 			duration;				///< Set for video
	// Parameters from the different investigative methods used
	FileType		file_type;
  std::string ext;            // File extension
  std::string magic_info;     // Information from libmagic
  std::string extraction_plugin; // Metametadata, plugin used by libextractor
  std::string other;						 // Misc info from libextractor
	bool	all_set;							// Set to abort further tests
	bool complete() {
		type = to_string(file_type);
		return all_set;				
	}
	int convert(std::string n) {
		std::cout << "convert " << n << std::endl;
		int i=0;
		try {
			i = std::stoi(n);
		}
		catch (const std::invalid_argument& ia) {
			std::cerr << "Invalid argument when converting from " << n << " to int: " << ia.what() << '\n';
		}
		return i;
	}
	void set_width(std::string w) {
		width = convert(w);				
	}
	void set_height(std::string h) {
		height = convert(h);				
	}
};

inline std::ostream& operator<<(std::ostream& os, const struct file_meta& m)
{
  os << m.path << " " << m.ext << " " << m.magic_info << " " << m.creation_date << " " 
     << " " << m.extraction_plugin << " " << m.other;
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

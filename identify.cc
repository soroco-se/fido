#include <identify.h>
#include <iomanip>  // Temp
#include <iostream> // Bara för debug
#include <exiv2/exiv2.hpp>

Identify::Identify()
{
}

Identify::~Identify()
{
  magic_close(magic_cookie);
  EXTRACTOR_plugin_remove_all (extractor_all);
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

  // Setup libextractor
  extractor_all = EXTRACTOR_plugin_add_defaults (EXTRACTOR_OPTION_DEFAULT_POLICY);
  extractor_img = EXTRACTOR_plugin_add(extractor_img, "exiv2",NULL, EXTRACTOR_OPTION_DEFAULT_POLICY);
  extractor_vid = EXTRACTOR_plugin_add(extractor_vid, "gstreamer",NULL, EXTRACTOR_OPTION_DEFAULT_POLICY);

  return ret;
}

file_meta Identify::process(std::string path)
{
  // Reset current file metadata
  current = file_meta(path);
	// Get a hint on what type of file this is
  current.magic_info = magic_file(magic_cookie, path.c_str());

  // Try to get metadata from the file
  if (current.magic_info.find("image data") != std::string::npos) {
    // This is an image
    current.file_type = FileType::IMAGE;
    if (current.magic_info.find("PNG") != std::string::npos) {
      // For PNG - magic finds all that can be found. No date available
      // Too old gcc, regex not available...
      std::size_t wpos = current.magic_info.find_first_of("0123456789");
      std::size_t wend = current.magic_info.find_first_not_of("0123456789",wpos);
      std::size_t hpos = current.magic_info.find_first_of("0123456789", wend);
      std::size_t hend = current.magic_info.find_first_not_of("0123456789",hpos);
      current.set_width(current.magic_info.substr(wpos,wend-wpos));
      current.set_height(current.magic_info.substr(hpos,hend-hpos));
      current.all_set = true;
    }
    else if (current.magic_info.find("TIFF") != std::string::npos) {
      // The RAW files dont display much metadata, if any
      current.all_set = true;
    }
    else if (current.magic_info.find("JPEG") != std::string::npos) {
      // Now use exiv2
      Exiv2::Image::AutoPtr image = Exiv2::ImageFactory::open(path);
      image->readMetadata();
      Exiv2::ExifData &exifData = image->exifData();
      if (exifData.empty()) {
        error = "No exif data in file";
      }
      current.width  = exifData.findKey(Exiv2::ExifKey("Exif.Image.ImageWidth"))->toLong();
      current.height = exifData.findKey(Exiv2::ExifKey("Exif.Image.ImageWidth"))->toLong();
      current.creation_date = exifData.findKey(Exiv2::ExifKey("Exif.Photo.DateTimeOriginal"))->toString();
      current.all_set = true;
      /*
      for (Exiv2::ExifData::const_iterator it = exifData.begin(); it != exifData.end(); ++it) {
        const char* tn = it->typeName();
        std::cout << std::setw(44) << std::setfill(' ') << std::left
          << it->key() << " "
          << "0x" << std::setw(4) << std::setfill('0') << std::right
          << std::hex << it->tag() << " "
          << std::setw(9) << std::setfill(' ') << std::left
          << (tn ? tn : "Unknown") << " "
          << std::dec << std::setw(3)
          << std::setfill(' ') << std::right
          << it->count() << "  " << std::dec
          << it->value() << "\n";
      }
      */
    }
    
  }

  // Check if we need to proceed
  if (current.complete()) 
    return current;

  struct EXTRACTOR_PluginList* extractor_plugins;

  if (current.magic_info.find("MPEG") != std::string::npos) {
    current.file_type = FileType::VIDEO; 
    extractor_plugins = extractor_vid;
  }
  else
    extractor_plugins = extractor_all;

  // Lambda to emulate a C-style function pointer
  EXTRACTOR_extract (extractor_plugins, path.c_str(), NULL, 0, 
    [](void *cls, const char *plugin_name, enum EXTRACTOR_MetaType type, enum EXTRACTOR_MetaFormat format, const char *data_mime_type, const char *data, size_t data_len) -> int {
      auto me = static_cast<Identify*>(cls);
      return me->meta(plugin_name, type, std::string(data, data_len));
    } , this);

  current.complete(); 
  return current;
}

// This is where we select and save metadata from the extractor
int Identify::meta(std::string plugin_name, enum EXTRACTOR_MetaType type, std::string data) 
{
  switch (type) {
    case EXTRACTOR_METATYPE_CREATION_DATE: current.creation_date = data; break;
    default:
      current.other.append(" ").append(EXTRACTOR_metatype_to_string(type)); break;
  }
  current.extraction_plugin = plugin_name;  
  
  return 0;
}

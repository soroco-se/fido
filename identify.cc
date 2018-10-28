#include <identify.h>

//#include <iostream> // Bara för debug

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
	current.magic_info = magic_file(magic_cookie, path.c_str());

  struct EXTRACTOR_PluginList* extractor_plugins;

  if (current.magic_info.find("JPEG") != std::string::npos)
    extractor_plugins = extractor_img;
  else if (current.magic_info.find("MPEG") != std::string::npos)
    extractor_plugins = extractor_vid;
  else
    extractor_plugins = extractor_all;

  // Lambda to emulate a C-style function pointer
  EXTRACTOR_extract (extractor_plugins, path.c_str(), NULL, 0, 
    [](void *cls, const char *plugin_name, enum EXTRACTOR_MetaType type, enum EXTRACTOR_MetaFormat format, const char *data_mime_type, const char *data, size_t data_len) -> int {
      auto me = static_cast<Identify*>(cls);
      return me->meta(plugin_name, type, std::string(data, data_len));
    } , this);
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

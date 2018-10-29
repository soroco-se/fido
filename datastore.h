#ifndef DATASTORE_H
#define DATASTORE_H

#include <sqlite3.h>

// Datastore will store file metadata
// Database table layout
// Where is the file now? | When was image/video created? | width of img? | height of img? | length of video? | Where should it go?
// filepath | type | creation_date | width | height | length | destination_path

enum FileType {
  OTHER = 0
  IMAGE = 1,
  VIDEO = 2,
  PDF   = 3,
  DOC   = 4,
  TEXT  = 5
};

class Datastore
{
  public: 
    Datastore();
    ~Datastore();
    
    void set_path(std::string p)  { path_to_db = p; }
    void set_name(std::string n)  { name_of_db = n; }
    bool open();                    ///<  Open, and initiate a new database.
    bool open(std::string db_path); ///< Open an existing database, RO?
    bool set(std::string path, FileType ft, std::string date, int w, int h, double dur, std::string dest);
    std::vector<std::string>  dump(); ///< Return all entries in db
    std::string get_error() { return error; }  
  
  private:
    std::string get_file_path();  // Return full file path based on path and name
    std::string path_to_db;
    std::string name_of_db;
    std::string file_path;
    std::string error;
    sqlite3*  db;
};

#endif

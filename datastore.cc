#include <datastore.h>
#include <sstream>


Datastore::Datastore() :
  path_to_db("."),
  name_of_db("datastore")
  {}
  
Datastore::~Datastore()
{
   sqlite3_close(db);
}

std::string Datastore::get_file_path()
{
  if (file_path.empty()) {
    std::time_t tp = std::time(NULL); // Get current time
    std::stringstream fp;
    fp << path_to_db << "/" << name_of_db << "_" << tp << ".db";
    file_path = fp.str();
  }
  return file_path;
}
    
//  Open, and initiate a new database.
bool Datastore::open()
{
  if (!open(get_file_path()))
    return false;
  
  std::stringstream sql;
  sql << "CREATE TABLE FIDO(ID INTEGER PRIMARY KEY     NOT NULL,";
  sql << "CURRENT_PATH TEXT    NOT NULL,";
  sql << "FILE_TYPE    TEXT     NOT NULL,";
  sql << "WIDHT        INTEGER,";
  sql << "HEIGHT       INTEGER,";
  sql << "DURATION         REAL,";
  sql << "DESTINATION_PATH TEXT);";
   char* errMsg=0;
   if ( sqlite3_exec(db, sql.str().c_str(), NULL, 0, &errMsg) != SQLITE_OK ) {
      error = errMsg;
      sqlite3_free(errMsg);
      return false;
   }
   return true;
}

// Open an existing database, RO?
bool Datastore::open(std::string db_path)
{
  if (sqlite3_open(path, &db) != 0) {
    std::stringstream err;
    err << "Failed to open database: " << path << " Error is: " << sqlite3_errmsg(db);
    error = err.str();
    return false;
  }
  return true;
}

bool Datastore::set(std::string path, FileType ft, std::string date, int w, int h, double dur, std::string dest)
{
   char *errMsg = 0;
   std::stringstream sql;
   sql << "INSERT INTO FIDO (CURRENT_PATH, FILE_TYPE, WIDTH, HEIGHT, DURATION, DESTINATION_PATH)";
   sql << "VALUES ('" << path << "'," << ft << "," << w << "," << h << "," << dur << ",'" << dest << "');"; 

   if ( sqlite3_exec(db, sql.str().c_Str(), NULL, 0, &errMsg) != SQLITE_OK ) {
      error = errMsg;
      sqlite3_free(errMsg);
      return false;
   }
   return true;
}

// Return all entries in db
std::vector<std::string>  Datastore::dump()
{
  char *errMsg = 0;
  std::stringstream sql;
  sql << "SELECT * FROM FIDO";
  std::vector<std:string> data;
   if ( sqlite3_exec(db, sql.str().c_Str(), [](void *data, int argc, char **argv, char **azColName){
        auto vec = static_cast<std::vector<std::string>*>(data);
        std::string row;
        for(i = 0; i<argc; i++){
          if (!row.empty())
            row += ", ";
            row += (argv[i] ? argv[i] : "NULL");
        }  
        vec.push_back(row);
      }, (void*)&data, &errMsg) != SQLITE_OK ) {
      error = errMsg;
      sqlite3_free(errMsg);
      return false;
   }
   return data;
}

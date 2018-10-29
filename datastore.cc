#include <datastore.h>
#include <sstream>
#include <cstring>  // memset


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
  sql << "CURRENT_PATH      TEXT    NOT NULL,";
  sql << "FILE_TYPE         TEXT     NOT NULL,";
  sql << "CREATION_DATE     TEXT     NOT NULL,";
  sql << "WIDTH             INTEGER,";
  sql << "HEIGHT            INTEGER,";
  sql << "DURATION          REAL,";
  sql << "DESTINATION_PATH  TEXT);";
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
  if (sqlite3_open(db_path.c_str(), &db) != 0) {
    std::stringstream err;
    err << "Failed to open database: " << db_path << " Error is: " << sqlite3_errmsg(db);
    error = err.str();
    return false;
  }
  return true;
}

std::string Datastore::format_date(std::string d)
{
  if (d.empty())
    return d;

  struct tm tm;
  char buf[255];

  memset(&tm, 0, sizeof(struct tm));
  strptime(d.c_str(), "%Y:%m:%d %H:%M:%S", &tm);
  strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", &tm);
  return std::string(buf);
}

bool Datastore::set(std::string path, std::string ft, std::string date, int w, int h, double dur, std::string dest)
{
   char *errMsg = 0;
   std::stringstream sql;
   sql << "INSERT INTO FIDO (CURRENT_PATH, FILE_TYPE, CREATION_DATE, WIDTH, HEIGHT, DURATION, DESTINATION_PATH)";
   sql << "VALUES ('" << path << "','" << ft << "','" << format_date(date) << "'," << w << "," << h << "," << dur << ",'" << dest << "');"; 

   if ( sqlite3_exec(db, sql.str().c_str(), NULL, 0, &errMsg) != SQLITE_OK ) {
      error = errMsg;
      sqlite3_free(errMsg);
      return false;
   }
   return true;
}

// Return all entries in db
bool Datastore::dump(std::vector<std::string>& ret_data)
{
  char *errMsg = 0;
  std::stringstream sql;
  sql << "SELECT * FROM FIDO";
  std::vector<std::string> data;
   if ( sqlite3_exec(db, sql.str().c_str(), 
     [](void *data, int argc, char **argv, char **azColName) -> int {
        auto vec = static_cast<std::vector<std::string>*>(data);
        std::string row;
        for(int i = 0; i<argc; i++){
          if (!row.empty())
            row += ", ";
            row += (argv[i] ? argv[i] : "NULL");
        }  
        vec->push_back(row);
        return 0;
      } , (void*)&data, &errMsg) != SQLITE_OK ) {
      error = errMsg;
      sqlite3_free(errMsg);
      return false;
   }
   ret_data=data;
   return true;
}

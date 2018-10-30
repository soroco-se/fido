#ifndef FIDOUTIL_H
#define FIDOUTIL_H

#include <string>
#include <sstream>
#include <stdexcept>

namespace fido {
// Conversions
int str_to_int(std::string n) {
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

double str_to_dbl(std::string n) {
  std::cout << "convert " << n << std::endl;
  double d=0.0;
  try {
    d = std::stod(n);
  }
  catch (const std::invalid_argument& ia) {
    std::cerr << "Invalid argument when converting from " << n << " to double: " << ia.what() << '\n';
  }
  return d;
}

  
std::string Datastore::format_date(std::string d)
{
  if (d.empty())
    return d;

  // Get rid of the separators
  int year, month, day, hour, min, sec;
  std::istringstream din(d);
  din >> year;
  din.ignore();
  din >> month;
  din.ignore();
  din >> day;
  din.ignore();
  din >> hour;
  din.ignore();
  din >> min;
  din.ignore();
  din >> sec;
  
  std::string new_date;
  // Attempt to validate the datetime
  struct tm dtm = {sec, min, hour, day, month-1, year-1900, 0, 0, 0};
  if (mktime(&dtm) != time_t(-1) &&
     dtm.year == year-1900 &&
     dtm.mon  == month-1 &&
     dtm.mday == day) {
    std::stringstream dout;
    dout << year << "-" << month << "-" << day << " " << hour << ":" << min << ":" << sec;
    new_date = dout.str();
  }
  else {
    // We have an invalid date, but we really need one. What do to?
    new_date = d;
  }
  return new_date;
}


}

#endif

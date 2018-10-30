#ifndef FIDOUTIL_H
#define FIDOUTIL_H

#include <string>
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


}

#endif

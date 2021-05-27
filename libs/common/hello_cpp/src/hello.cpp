#include "libhello_cpp/hello.hpp"

#include <iostream>
#include <string>

namespace common {

Hello::Hello(const std::string& user) : user_(user) {}

void Hello::printMsg(const std::string& msg) { std::cout << msg << " " << user_ << std::endl; }

}  // namespace common

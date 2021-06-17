#include "libhello_cpp/hello.hpp"

#include <iostream>
#include <string>

namespace common {

Hello::Hello(std::string user) : user_(std::move(user)) {}

void Hello::printMsg(const std::string& msg) { std::cout << msg << " " << user_ << std::endl; }

}  // namespace common

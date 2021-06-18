#pragma once

#include <string>

namespace common {

class Hello {
 public:
  explicit Hello(std::string user = "Guest");
  void printMsg(const std::string& msg);

 private:
  std::string user_;
};

}  // namespace common

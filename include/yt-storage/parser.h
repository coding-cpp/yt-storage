#pragma once

#include <iostream>
#include <map>
#include <string>

#include <logger/log.h>

namespace yt {

class Parser {
private:
  std::map<std::string, std::pair<std::string, std::string>> flags;
  std::map<std::string, std::string> values;

  void validate();

public:
  Parser();
  ~Parser();

  void add(const std::string &flag, const std::string &key,
           const std::string &description = "",
           const std::string &defaultValue = "");
  void parse(int argc, char **argv);
  std::string get(const std::string &key);
};

} // namespace yt
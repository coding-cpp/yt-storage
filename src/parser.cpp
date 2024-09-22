#include <yt-storage/parser.h>

yt::Parser::Parser() { return; }

yt::Parser::~Parser() { return; }

void yt::Parser::add(const std::string &flag, const std::string &key,
                     const std::string &description,
                     const std::string &defaultValue) {
  this->flags[flag] = std::make_pair(key, description);
  if (defaultValue != "") {
    this->values[key] = defaultValue;
  }

  return;
}

void yt::Parser::parse(int argc, char **argv) {
  for (int i = 1; i < argc; i++) {
    if (this->flags.find(argv[i]) != this->flags.end()) {
      if (i + 1 < argc) {
        this->values[this->flags[argv[i]].first] = argv[i + 1];
        i++;
      }
    }
  }

  this->validate();
  return;
}

std::string yt::Parser::get(const std::string &key) {
  return this->values[key];
}

void yt::Parser::validate() {
  for (std::pair<const std::string, std::pair<std::string, std::string>>
           element : this->flags) {
    if (this->values.find(element.second.first) == this->values.end()) {
      logger::warning("Missing argument: " + element.first + " - " +
                      element.second.second + ": ");

      std::string value;
      std::cin >> value;
      this->values[element.second.first] = value;
      std::cout << std::endl;
    }
  }

  return;
}
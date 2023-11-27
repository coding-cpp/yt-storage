#pragma once

#include <bitset>
#include <fstream>
#include <string>

#include <yt-storage/print.h>

namespace yt {

class Decrypter {
private:
  std::ifstream inputFile;

public:
  Decrypter(const std::string &inputFilePath);
  ~Decrypter();

  void dump(const std::string &outputFilePath);
};

} // namespace yt
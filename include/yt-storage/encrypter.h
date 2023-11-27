#pragma once

#include <bitset>
#include <fstream>
#include <string>

#include <yt-storage/print.h>

namespace yt {

class Encrypter {
private:
  std::ifstream inputFile;

  bool eof();

public:
  Encrypter(const std::string &inputFilePath);
  ~Encrypter();

  void setEncryptionKey(const std::string &encryptionKey);
  void dump(const std::string &outputFilePath);
};

} // namespace yt

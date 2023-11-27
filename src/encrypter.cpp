#include <yt-storage/encrypter.h>

yt::Encrypter::Encrypter(const std::string &inputFilePath) {
  this->inputFile.open(inputFilePath, std::ios::binary);
  if (!this->inputFile.is_open()) {
    print::error(inputFilePath);
    throw std::runtime_error("Failed to open input file!");
  }

  return;
}

yt::Encrypter::~Encrypter() {
  if (this->inputFile.is_open())
    this->inputFile.close();

  return;
}

void yt::Encrypter::dump(const std::string &outputFilePath) {
  std::fstream outputFile(outputFilePath, std::ios::out);

  char byte;
  while (this->inputFile.read(&byte, sizeof(byte))) {
    std::bitset<8> bits(byte);
    outputFile << bits.to_string();
  }

  outputFile.close();

  return;
}

bool yt::Encrypter::eof() { return this->inputFile.eof(); }
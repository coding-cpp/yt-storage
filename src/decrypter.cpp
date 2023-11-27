#include <yt-storage/decrypter.h>

yt::Decrypter::Decrypter(const std::string &inputFilePath) {
  this->inputFile.open(inputFilePath);
  if (!this->inputFile.is_open()) {
    print::error(inputFilePath);
    throw std::runtime_error("Failed to open input file!");
  }

  return;
}

yt::Decrypter::~Decrypter() {
  this->inputFile.close();

  return;
}

void yt::Decrypter::dump(const std::string &outputFilePath) {
  std::ofstream outputFile(outputFilePath, std::ios::binary);

  char byte;
  std::string binaryString;
  while (this->inputFile >> binaryString) {
    for (size_t i = 0; i < binaryString.size(); i += 8) {
      std::string byteString = binaryString.substr(i, 8);
      std::bitset<8> bits(byteString);
      byte = static_cast<char>(bits.to_ulong());
      outputFile.write(&byte, sizeof(byte));
    }
  }

  outputFile.close();
  return;
}
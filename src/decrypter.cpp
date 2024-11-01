#include <yt-storage/decrypter.h>

yt::Decrypter::Decrypter() : col(0), row(0) { return; }

yt::Decrypter::~Decrypter() {
  if (this->cap.isOpened()) {
    this->cap.release();
  }
  return;
}

void yt::Decrypter::decrypt(yt::options options) {
  this->options = options;
  this->options.width = -1;
  this->options.height = -1;
  if (!brewtils::os::file::exists(this->options.inputFile)) {
    logger::error("Input file " + this->options.inputFile + " does not exist",
                  "void yt::Decrypter::decrypt(yt::options options)");
  }

  this->cap.open(this->options.inputFile);
  if (!this->cap.isOpened()) {
    logger::error("Could not open file " + this->options.inputFile,
                  "void yt::Decrypter::decrypt(yt::options options)");
  }

  this->readMetadata();
  this->readFiledata();
  return;
}

bool yt::Decrypter::getData() {
  if (this->row == 0 && this->col == 0) {
    bool isFrameRead = this->cap.read(this->frame);
    if (!isFrameRead) {
      logger::error("Could not read frame", "bool yt::Decrypter::getData()");
    }
    if (this->options.width == -1 || this->options.height == -1) {
      this->options.width = this->frame.size().width;
      this->options.height = this->frame.size().height;
    }
  }

  bool data = this->frame.at<cv::Vec3b>(this->row, this->col)[0] > 128;
  this->col++;
  if (this->col == this->options.width) {
    this->col = 0;
    row++;
  }

  if (this->row == this->options.height) {
    this->row = 0;
  }

  return data;
}

void yt::Decrypter::readMetadata() {
  uint16_t jsonDataSize = 0;
  this->row = 0;
  this->col = 0;
  for (int i = 0; i < 16; i++) {
    jsonDataSize += this->getData() ? std::pow(2, i) : 0;
  }

  if (jsonDataSize % 8 != 0) {
    logger::error("Invalid metadata size",
                  "void yt::Decrypter::readMetadata()");
  }

  std::vector<bool> jsonData = std::vector<bool>(jsonDataSize);
  for (int i = 0; i < jsonDataSize; i++) {
    jsonData[i] = this->getData();
  }

  std::string strigifiedJson = "";
  for (int i = 0; i < jsonData.size(); i += 8) {
    int asciiValue = 0;
    for (size_t j = 0; j < 8 && (i + j) < jsonData.size(); ++j) {
      asciiValue = (asciiValue << 1) | static_cast<int>(jsonData[i + j]);
    }
    strigifiedJson += static_cast<char>(asciiValue);
  }

  try {
    json::parser parser;
    json::object data = parser.loads(strigifiedJson);
    this->options.outputFile =
        "../files/" + static_cast<std::string>(data["name"]);
    this->fileSize = data["size"];
  } catch (const std::exception &e) {
    logger::error("Invalid metadata detected",
                  "void yt::Decrypter::readMetadata()");
  }

  if (fileSize % 8 != 0) {
    logger::error("Invalid file size", "void yt::Decrypter::readMetadata()");
  }
  return;
}

void yt::Decrypter::readFiledata() {
  std::ofstream file(this->options.outputFile, std::ios::binary);
  if (!file.is_open()) {
    logger::error("Could not open file " + this->options.outputFile,
                  "void yt::Decrypter::readFiledata()");
  }

  char byte = 0;
  for (long long int i = 0; i < this->fileSize; i++) {
    byte = (byte << 1) | this->getData();
    if ((i + 1) % 8 == 0) {
      file.put(byte);
      byte = 0;
    }
  }

  file.close();
  return;
}
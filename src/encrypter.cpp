#include <yt-storage/encrypter.h>

yt::Encrypter::Encrypter() : col(0), row(0) { return; }

yt::Encrypter::~Encrypter() {
  if (this->writer.isOpened()) {
    this->writer.release();
  }
  return;
}

void yt::Encrypter::encrypt(yt::options options) {
  this->options = options;
  if (!brewtils::os::file::exists(this->options.inputFile)) {
    logger::error("Input file " + this->options.inputFile + " does not exist",
                  "void yt::Encrypter::encrypt(yt::options options)");
  }

  this->writer.open(this->options.outputFile,
                    cv::VideoWriter::fourcc('m', 'p', '4', 'v'),
                    this->options.fps,
                    cv::Size(this->options.width, this->options.height), false);
  if (!this->writer.isOpened()) {
    logger::error("Could not open video writer",
                  "void yt::Encrypter::encrypt(yt::options options)");
  }

  this->setMetadata();
  this->setFiledata();

  if (this->row != 0 || this->col != 0) {
    this->writer.write(this->frame);
  }

  return;
}

std::vector<bool> yt::Encrypter::getJsonData() {
  std::vector<bool> response;
  json::object data;
  data["name"] = brewtils::os::basePath(this->options.inputFile);
  data["size"] =
      (long long int)(brewtils::os::file::size(this->options.inputFile) * 8);
  data["version"] = yt::version;
  const std::string stringifiedJson = data.dumps();
  int asciiValue, i;
  for (char ch : stringifiedJson) {
    asciiValue = static_cast<int>(ch);
    for (i = 7; i >= 0; --i) {
      response.push_back((asciiValue >> i) & 1);
    }
  }
  return response;
}

void yt::Encrypter::setMetadata() {
  const std::vector<bool> jsonData = yt::Encrypter::getJsonData();
  uint16_t jsonDataSize = jsonData.size();

  for (int i = 0; i < 16; i++) {
    this->setData(jsonDataSize % 2 != 0);
    jsonDataSize /= 2;
  }

  for (const bool &bit : jsonData) {
    this->setData(bit);
  }

  return;
}

void yt::Encrypter::setFiledata() {
  std::ifstream file(this->options.inputFile, std::ios::binary);
  if (!file.is_open()) {
    logger::error("Could not open file " + this->options.inputFile,
                  "void yt::Encrypter::setFiledata()");
  }

  char byte;
  while (file.get(byte)) {
    for (int i = 7; i >= 0; --i) {
      this->setData((byte >> i) & 1);
    }
  }

  file.close();
  return;
}

void yt::Encrypter::setData(bool data) {
  for (int i = 0; i < yt::REDUNDANCY; i++) {
    if (this->row == 0 && this->col == 0) {
      this->frame = cv::Mat::zeros(
          cv::Size(this->options.width, this->options.height), CV_8UC1);
    }

    this->frame.at<uchar>(this->row, this->col) = data ? 255 : 0;
    this->col++;
    if (this->col == this->options.width) {
      this->col = 0;
      this->row++;
    }

    if (this->row == this->options.height) {
      this->writer.write(this->frame);
      this->row = 0;
    }
  }
  return;
}
#include <yt-storage/encrypter.h>

yt::Encrypter::Encrypter() : col(0), row(0) { return; }

yt::Encrypter::~Encrypter() {
  if (this->zipper != nullptr) {
    delete this->zipper;
  }
  if (this->writer.isOpened()) {
    this->writer.release();
  }
  return;
}

void yt::Encrypter::encrypt(yt::options options) {
  this->options = options;
  if (!brewtils::os::file::exists(this->options.inputPath) &&
      !brewtils::os::dir::exists(this->options.inputPath)) {
    logger::error("Input path " + this->options.inputPath + " does not exist",
                  "void yt::Encrypter::encrypt(yt::options options)");
  }

  this->writer.open(
      brewtils::os::joinPath(this->options.outputDir, this->options.outputFile),
      cv::VideoWriter::fourcc('m', 'p', '4', 'v'), this->options.fps,
      cv::Size(this->options.width, this->options.height), false);
  if (!this->writer.isOpened()) {
    logger::error("Could not open video writer",
                  "void yt::Encrypter::encrypt(yt::options options)");
  }

  this->setMetadata();
  if (this->zipper == nullptr) {
    this->setFiledata(this->options.inputPath);
  } else {
    while (!this->zipper->isFinished()) {
      this->setStringData(this->zipper->getHeader());
      this->setFiledata(this->zipper->getCurrentFile());
    }
    this->setStringData(this->zipper->getFooter());
  }

  if (this->row != 0 || this->col != 0) {
    this->writer.write(this->frame);
  }

  return;
}

std::string yt::Encrypter::getJsonData() {
  std::vector<bool> response;
  json::object data;
  if (brewtils::os::file::exists(this->options.inputPath)) {
    data["name"] = brewtils::os::basePath(this->options.inputPath);
    data["size"] =
        (long long int)(brewtils::os::file::size(this->options.inputPath) * 8);
  } else if (brewtils::os::dir::exists(this->options.inputPath)) {
    this->zipper = new zippuccino::Zipper();
    zipper->add(this->options.inputPath);
    data["name"] = brewtils::os::basePath(this->options.inputPath) + ".zip";
    data["size"] = (long long int)(this->zipper->getSize() * 8);
  } else {
    logger::error("Input path " + this->options.inputPath +
                      " is neither a file nor a directory",
                  "std::vector<bool> yt::Encrypter::getJsonData()");
  }
  data["version"] = yt::version;
  return data.dumps();
}

void yt::Encrypter::setMetadata() {
  const std::string jsonData = yt::Encrypter::getJsonData();
  uint16_t jsonDataSize = jsonData.size() * 8;

  for (int i = 0; i < 16; i++) {
    this->setData(jsonDataSize % 2 != 0);
    jsonDataSize /= 2;
  }

  this->setStringData(jsonData);
  return;
}

void yt::Encrypter::setFiledata(const std::string &filePath) {
  std::ifstream file(filePath, std::ios::binary);
  if (!file.is_open()) {
    logger::error("Could not open file " + filePath,
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

void yt::Encrypter::setStringData(const std::string &data) {
  for (char ch : data) {
    for (int i = 7; i >= 0; --i) {
      this->setData((ch >> i) & 1);
    }
  }
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
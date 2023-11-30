#include <yt-storage/encrypter.h>

yt::Encrypter::Encrypter(const std::string &inputFilePath, int width) {
  this->inputFile.open(inputFilePath, std::ios::binary);
  if (!this->inputFile.is_open()) {
    print::error(inputFilePath);
    throw std::runtime_error("Failed to open input file!");
  }

  this->resolution = cv::Size(width, width * 9 / 16);
  this->frame = cv::Mat::zeros(this->resolution, CV_8UC1);

  return;
}

yt::Encrypter::~Encrypter() {
  this->video.release();
  if (this->inputFile.is_open())
    this->inputFile.close();

  return;
}

void yt::Encrypter::dump(const std::string &outputFilePath) {
  this->video = cv::VideoWriter(outputFilePath,
                                cv::VideoWriter::fourcc('M', 'J', 'P', 'G'), 30,
                                this->resolution, false);
  if (!this->video.isOpened()) {
    print::error(outputFilePath);
    throw std::runtime_error("Failed to open output file!");
  }

  char byte;
  short currRow = 0, currCol = 0;
  while (inputFile.read(&byte, sizeof(byte))) {
    std::bitset<8> bits(byte);
    if (currCol + currRow * this->resolution.width + 8 >
        this->resolution.area()) {
      this->video.write(this->frame);
      this->frame = cv::Mat::zeros(this->resolution, CV_8UC1);
      currCol = 0;
      currRow = 0;
    }
    for (int i = 0; i < 8; i++) {
      this->frame.at<uchar>(currRow, currCol) = 255 * bits[i];
      if (currCol == this->resolution.width - 1) {
        currCol = 0;
        currRow += 1;
      } else {
        currCol += 1;
      }
    }
  }
  if (!this->frame.empty()) {
    this->video.write(this->frame);
  }

  int lastFrameUsedBits = currCol + currRow * this->resolution.width;
  this->appendLastFrame(lastFrameUsedBits);

  return;
}

void yt::Encrypter::appendLastFrame(int lastFrameUsedBits) {
  this->frame = cv::Mat::zeros(this->resolution, CV_8UC1);
  short currRow = 0, currCol = 0;
  for (int i = 31; i >= 0; i--) {
    int bit = (lastFrameUsedBits >> i) & 1;
    this->frame.at<uchar>(currRow, currCol) = 255 * bit;
    if (currCol == this->resolution.width - 1) {
      currCol = 0;
      currRow += 1;
    } else {
      currCol += 1;
    }
  }

  this->video.write(this->frame);
  return;
}
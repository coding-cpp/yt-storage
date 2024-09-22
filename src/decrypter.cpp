#include <yt-storage/decrypter.h>

yt::Decrypter::Decrypter(const std::string &inputFilePath) {
  this->cap = cv::VideoCapture(inputFilePath);
  if (!this->cap.isOpened()) {
    logger::error("Failed to open input file: " + inputFilePath,
                  "yt::Decrypter::Decrypter(const std::string &inputFilePath)");
  }

  this->extractInfo();
  return;
}

yt::Decrypter::~Decrypter() {
  this->cap.release();

  return;
}

void yt::Decrypter::dump(const std::string &outputFilePath) {
  std::ofstream outputFile;
  outputFile.open(outputFilePath, std::ios::binary);
  if (!outputFile.is_open()) {
    logger::error(
        "Failed to open output file: " + outputFilePath,
        "void yt::Decrypter::dump(const std::string &outputFilePath)");
  }

  char byte;
  char pixel;
  bool bit;
  short currRow, currCol;
  std::bitset<8> bits;
  this->totalFrames -= 1;
  while (this->totalFrames--) {
    currRow = 0;
    currCol = 0;
    cap.read(this->frame);
    cv::cvtColor(this->frame, this->grayscale, cv::COLOR_BGR2GRAY);

    while (currCol + currRow * this->resolution.width + 8 <
           this->resolution.area()) {
      if ((this->totalFrames == 0) &&
          (currCol + currRow * this->resolution.width >=
           this->lastFrameUsedBits)) {
        break;
      }
      for (int i = 0; i < 8; i++) {
        pixel = this->grayscale.at<uchar>(currRow, currCol);
        bit = pixel < 0 ? 1 : 0;
        bits[i] = bit;

        if (currCol == this->resolution.width - 1) {
          currCol = 0;
          currRow += 1;
        } else {
          currCol += 1;
        }
      }

      byte = static_cast<char>(bits.to_ulong());
      outputFile.put(byte);
    }
  }

  outputFile.close();
  return;
}

void yt::Decrypter::extractInfo() {
  this->totalFrames = static_cast<int>(this->cap.get(cv::CAP_PROP_FRAME_COUNT));

  this->cap.set(cv::CAP_PROP_POS_FRAMES, totalFrames - 1);
  this->cap.read(this->frame);
  if (this->frame.empty()) {
    logger::error("Can't read empty frame!",
                  "void yt::Decrypter::extractInfo()");
  }

  this->resolution = this->frame.size();
  cv::cvtColor(this->frame, this->grayscale, cv::COLOR_BGR2GRAY);

  short currRow = 0, currCol = 0;
  this->lastFrameUsedBits = 0;
  char pixel;
  short bit;
  for (int i = 31; i >= 0; i--) {
    pixel = this->grayscale.at<uchar>(currRow, currCol);
    bit = pixel < 0 ? 1 : 0;
    this->lastFrameUsedBits += bit * std::pow(2, i);

    if (currCol == this->resolution.width - 1) {
      currCol = 0;
      currRow += 1;
    } else {
      currCol += 1;
    }
  }

  this->cap.set(cv::CAP_PROP_POS_FRAMES, 0);
  return;
}
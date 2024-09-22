#pragma once

#include <bitset>
#include <fstream>
#include <string>

#include <opencv2/opencv.hpp>

#include <logger/log.h>

namespace yt {

class Decrypter {
private:
  int totalFrames;
  unsigned int lastFrameUsedBits;

  cv::Size resolution;
  cv::Mat frame;
  cv::Mat grayscale;
  cv::VideoCapture cap;

  void extractInfo();

public:
  Decrypter(const std::string &inputFilePath);
  ~Decrypter();

  void dump(const std::string &outputFilePath);
};

} // namespace yt
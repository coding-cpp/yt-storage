#pragma once

#include <bitset>
#include <fstream>
#include <string>

#include <opencv2/opencv.hpp>

#include <logger/log.h>

namespace yt {

class Encrypter {
private:
  std::ifstream inputFile;

  cv::Size resolution;
  cv::Mat frame;
  cv::VideoWriter video;

  void appendLastFrame(int lastFrameUsedBits);

public:
  Encrypter(const std::string &inputFilePath, int width);
  ~Encrypter();

  void dump(const std::string &outputFilePath);
};

} // namespace yt
#pragma once

#include <yt-storage/options.h>

namespace yt {

class Decrypter {
private:
  int row, col;
  long long int fileSize;

  cv::Mat frame;
  cv::VideoCapture cap;

  yt::options options;

  bool getData();
  void readMetadata();
  void readFiledata();

public:
  Decrypter();
  ~Decrypter();

  void decrypt(yt::options options);
};

} // namespace yt
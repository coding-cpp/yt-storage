#pragma once

#include <zippuccino/zipper.h>

#include <yt-storage/options.h>

namespace yt {

class Encrypter {
private:
  int row, col;
  cv::Mat frame;
  cv::VideoWriter writer;

  yt::options options;

  std::string getJsonData();
  void setMetadata();
  void setFiledata(const std::string &filePath);
  void setStringData(const std::string &data);
  void setData(bool data);

  zippuccino::Zipper *zipper;

public:
  Encrypter();
  ~Encrypter();

  void encrypt(yt::options options);
};

} // namespace yt
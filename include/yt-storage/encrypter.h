#pragma once

#include <yt-storage/options.h>

namespace yt {

class Encrypter {
private:
  int row, col;
  cv::Mat frame;
  cv::VideoWriter writer;

  yt::options options;

  std::vector<bool> getJsonData();
  void setMetadata();
  void setFiledata();
  void setData(bool data);

public:
  Encrypter();
  ~Encrypter();

  void encrypt(yt::options options);
};

} // namespace yt
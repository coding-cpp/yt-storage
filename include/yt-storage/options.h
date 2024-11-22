#pragma once

#include <fstream>
#include <opencv2/opencv.hpp>

#include <brewtils/os.h>
#include <json/parse.h>

#include <yt-storage/version.h>

namespace yt {

const int REDUNDANCY = 7;

typedef struct {
  int width;
  int height;
  int fps;
  std::string inputPath;
  std::string outputFile;
  std::string outputDir;
} options;

} // namespace yt
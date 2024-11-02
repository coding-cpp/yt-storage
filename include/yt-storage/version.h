#pragma once

#include <string>

namespace yt {

const int MAJOR_VERSION = 0;
const int MINOR_VERSION = 1;
const int PATCH_VERSION = 0;

const std::string version = std::to_string(MAJOR_VERSION) + "." +
                            std::to_string(MINOR_VERSION) + "." +
                            std::to_string(PATCH_VERSION);

} // namespace yt
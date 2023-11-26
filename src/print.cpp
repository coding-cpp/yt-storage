#include <yt-storage/print.h>

void yt::print::success(const std::string &message, bool newLine) {
  std::cout << GREEN << "[SUCCESS]: " << RESET << message;
  if (newLine)
    std::cout << std::endl;

  return;
}

void yt::print::info(const std::string &message, bool newLine) {
  std::cout << BLUE << "[INFO]   : " << RESET << message;
  if (newLine)
    std::cout << std::endl;

  return;
}

void yt::print::warning(const std::string &message, bool newLine) {
  std::cout << YELLOW << "[WARNING]: " << RESET << message;
  if (newLine)
    std::cout << std::endl;

  return;
}

void yt::print::error(const std::string &message, bool newLine) {
  std::cerr << RED << "[ERROR]  : " << RESET << message;
  if (newLine)
    std::cerr << std::endl;

  return;
}

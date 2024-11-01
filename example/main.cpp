#include <brewtils/env.h>

#include <yt-storage/decrypter.h>
#include <yt-storage/encrypter.h>

int main(int argc, char **argv) {
  yt::options options;
  options.width = std::stoi(brewtils::env::get("WIDTH", "1920"));
  options.height = std::stoi(brewtils::env::get("HEIGHT", "1080"));
  options.fps = std::stoi(brewtils::env::get("FPS", "30"));
  options.inputFile = brewtils::env::get("INPUT_FILE");
  options.outputFile = brewtils::env::get("OUTPUT_FILE", "../files/output.mp4");
  if (options.inputFile.empty()) {
    logger::error("Input file not provided", "int main(int argc, char **argv)");
  }

  int mode = std::stoi(brewtils::env::get("MODE", "0"));
  switch (mode) {
  case 0: {
    yt::Encrypter encrypter;
    encrypter.encrypt(options);
    break;
  }
  case 1: {
    yt::Decrypter decrypter;
    decrypter.decrypt(options);
    break;
  }
  default:
    logger::error("Invalid mode: " + std::to_string(mode),
                  "int main(int argc, char **argv)");
  }

  return EXIT_SUCCESS;
}
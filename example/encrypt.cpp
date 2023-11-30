#include <iostream>

#include <yt-storage/encrypter.h>
#include <yt-storage/parser.h>

int main(int argc, char **argv) {
  yt::Parser parser;
  parser.add("-i", "input", "input file path");
  parser.add("--input", "input", "input file path");
  parser.add("-o", "output", "output file path", "output.mp4");
  parser.add("--output", "output", "output file path", "output.mp4");
  parser.add("-r", "resolution", "resolution of the resulting video", "1080");
  parser.add("--resolution", "resolution", "resolution of the resulting video",
             "1080");
  parser.parse(argc, argv);

  yt::Encrypter encrypter(parser.get("input"),
                          std::stoi(parser.get("resolution")));
  encrypter.dump(parser.get("output"));

  return EXIT_SUCCESS;
}
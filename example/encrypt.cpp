#include <iostream>

#include <yt-storage/encrypter.h>
#include <yt-storage/parser.h>

int main(int argc, char **argv) {
  yt::Parser parser;
  parser.add("-i", "input", "input file path");
  parser.add("--input", "input", "input file path");
  parser.add("-o", "output", "output file path", "output.mp4");
  parser.add("--output", "output", "output file path", "output.mp4");
  parser.add("-k", "key", "key to be used for encryption");
  parser.add("--key", "key", "key to be used for encryption");
  parser.add("-r", "resolution", "resolution of the resulting video", "1080");
  parser.add("--resolution", "resolution", "resolution of the resulting video",
             "1080");
  parser.parse(argc, argv);

  return EXIT_SUCCESS;
}
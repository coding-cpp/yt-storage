#include <iostream>

#include <yt-storage/decrypter.h>
#include <yt-storage/parser.h>

int main(int argc, char **argv) {
  yt::Parser parser;
  parser.add("-i", "input", "input file path", "output.mp4");
  parser.add("--input", "input", "input file path", "output.mp4");
  parser.add("-o", "output", "output file path");
  parser.add("--output", "output", "output file path");
  parser.parse(argc, argv);

  yt::Decrypter decrypter(parser.get("input"));
  decrypter.dump(parser.get("output"));

  return EXIT_SUCCESS;
}
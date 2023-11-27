#include <iostream>

#include <yt-storage/parser.h>

int main(int argc, char **argv) {
  yt::Parser parser;
  parser.add("-i", "input", "input file path");
  parser.add("--input", "input", "input file path");
  parser.add("-o", "output", "output file path");
  parser.add("--output", "output", "output file path");
  parser.parse(argc, argv);

  return EXIT_SUCCESS;
}
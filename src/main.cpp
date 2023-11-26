#include <iostream>

#include <yt-storage/parser.h>

int main(int argc, char **argv) {
  yt::Parser parser;
  parser.addFlag("-i", "input", "input file path");
  parser.addFlag("--input", "input", "input file path");
  parser.addFlag("-o", "output", "output file path");
  parser.addFlag("--output", "output", "output file path");
  parser.parse(argc, argv);

  return EXIT_SUCCESS;
}
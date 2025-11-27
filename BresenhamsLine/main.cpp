#include "model.h"
#include <iostream>

int main(int argc, char **argv) {
  string filename;
  int scale = 1;
  int padding = 0;

  for (int i = 1; i < argc; i++) {
    string input = argv[i];

    if ((input == "-f" || input == "--file") && i + 1 < argc)
      filename = argv[i + 1];
    else if ((input == "-s" || input == "--scale") && i + 1 < argc)
      scale = stoi(argv[i + 1]);
    else if ((input == "-p" || input == "--padding") && i + 1 < argc)
      padding = stoi(argv[i + 1]);
    else if (input == "-h" || input == "--help") {
      cout << "-f, --file: filename\n";
      cout << "-s, --scale: model scaling\n";
      cout << "-p, --padding: padding around model\n";

      return 0;
    }
  }

  if (filename.empty()) {
    cerr << "No input file\n";
    return 1;
  } else if (scale < 0) {
    cerr << "Invalid scale\n";
    return 1;
  } else if (padding < 0) {
    cerr << "Invalid padding\n";
    return 1;
  }

  Model model(filename, scale, padding);
  model.construct();

  return 0;
}

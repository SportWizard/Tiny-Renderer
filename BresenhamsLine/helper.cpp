#include "helper.h"

#include <sstream>
#include <stdexcept>

vector<string> readFile(string &filename) {
  vector<string> lines = {};
  ifstream f(filename);

  if (!f.is_open())
    throw invalid_argument("Invalid filename");

  string line;
  string word;

  while (getline(f, line))
    lines.push_back(line);

  f.close();

  return lines;
}

vector<string> split(string &s, char delimiter) {
  vector<string> result = {};

  try {
    stringstream ss(s);
    string t;

    while (getline(ss, t, delimiter))
      result.push_back(t);
  } catch (int e) {
    throw invalid_argument("Error splitting string");
  }

  return result;
}

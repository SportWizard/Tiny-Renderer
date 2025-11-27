#ifndef MODEL_H
#define MODEL_H

#include "tgaimage.h"
#include <string>
#include <tuple>
#include <vector>

using namespace std;

const TGAColor white = {255, 255, 255, 255}; // attention, BGRA order
const TGAColor red = {0, 0, 255, 255};

struct Vertex3 {
  double x, y, z;
};

class Model {
private:
  TGAImage framebuffer;
  vector<Vertex3> vertices;
  vector<int> faces; // Polygons connecting the vertices
  int scale;
  int padding;

  void extractData(vector<string> &data);
  void normalize();
  void plot(Vertex3 &v);
  void line(double ax, double ay, double bx, double by, TGAColor color);

public:
  Model(string filename, int scale, int padding);
  vector<Vertex3> getVertices();
  vector<int> getFaces();
  void construct();
};

#endif // !MODEL_H

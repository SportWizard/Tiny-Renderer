#include "model.h"

#include "helper.h"
#include "tgaimage.h"
#include <algorithm>
#include <cmath>
#include <string>
#include <vector>

Model::Model(string filename, int scale, int padding) {
  vector<string> data = readFile(filename);
  this->extractData(data);

  this->normalize();
  this->scaleModel(scale);
  this->addingPadding(padding);
  this->convertToInt();

  // The use of 1 is because of normalization convert the value between [0, 1],
  // then multiple the scaling plus padding
  this->framebuffer = TGAImage(scale + padding, scale + padding, TGAImage::RGB);
}

void Model::extractData(vector<string> &data) {
  for (string datum : data) {
    if (datum[0] != 'v' && datum[0] != 'f')
      continue;

    vector<string> values = split(datum, ' ');
    string type = values[0];

    if (type == "v")
      this->vertices.push_back(
          Vertex3{stod(values[1]), stod(values[2]), stod(values[3])});
    else if (type == "f") {
      for (int i = 1; i < values.size(); i++)
        this->faces.push_back(stoi(split(values[i], '/')[0]));
    }
  }
}

void Model::normalize() {
  if (this->vertices.empty())
    return;

  double minX = this->vertices[0].x, maxX = this->vertices[0].x;
  double minY = this->vertices[0].y, maxY = this->vertices[0].y;
  // double minZ = this->vertices[0].z, maxZ = this->vertices[0].z;

  for (Vertex3 &v : this->vertices) {
    minX = min(minX, v.x);
    maxX = max(maxX, v.x);
    minY = min(minY, v.y);
    maxY = max(maxY, v.y);
    // minZ = min(minZ, v.z);
    // maxZ = max(maxZ, v.z);
  }

  double rangeX = maxX - minX;
  double rangeY = maxY - minY;
  // double rangeZ = maxZ - minZ;

  for (Vertex3 &v : this->vertices) {
    v.x = (rangeX != 0.0) ? (v.x - minX) / rangeX : 0.0;
    v.y = (rangeY != 0.0) ? (v.y - minY) / rangeY : 0.0;
    // v.z = (rangeZ != 0.0) ? (v.z - minZ) / rangeZ : 0.0;
  }
}

void Model::scaleModel(int scale) {
  for (Vertex3 &v : this->vertices) {
    v.x *= scale;
    v.y *= scale;
    // v.z *= scale;
  }
}

void Model::addingPadding(int padding) {
  padding = round(padding / 2.0);

  for (Vertex3 &v : this->vertices) {
    v.x += padding;
    v.y += padding;
    // v.z += padding;
  }
}

void Model::convertToInt() {
  for (Vertex3 &v : this->vertices) {
    v.x = round(v.x);
    v.y = round(v.y);
    // v.z = round(v.z);
  }
}

void Model::plot(Vertex3 &v) { this->framebuffer.set(v.x, v.y, white); }

void Model::line(double ax, double ay, double bx, double by, TGAColor color) {
  bool steep = abs(ax - bx) < abs(ay - by);

  // if the line is steep, we transpose the image
  // because the program draws horizontal line better than vertical line
  if (steep) {
    swap(ax, ay);
    swap(bx, by);
  }

  if (ax > bx) {
    swap(ax, bx);
    swap(ay, by);
  }

  for (int x = ax; x <= bx; x++) {
    double t = (x - ax) / static_cast<double>(bx - ax);
    int y = round(ay + t * (by - ay));

    if (steep)
      // if transposed, de-transpose
      this->framebuffer.set(y, x, color);
    else
      this->framebuffer.set(x, y, color);
  }
}

vector<Vertex3> Model::getVertices() {
  vector<Vertex3> vertices = {};

  for (Vertex3 &v : this->vertices)
    vertices.push_back(Vertex3{v.x, v.y, v.z});

  return vertices;
}

vector<int> Model::getFaces() {
  vector<int> faces = {};

  for (int &face : this->faces)
    faces.push_back(face);

  return faces;
}

void Model::construct() {
  for (Vertex3 &v : this->vertices)
    this->plot(v);

  for (int i = 0; i < this->faces.size(); i += 3) {
    Vertex3 v1 = this->vertices[this->faces[i] - 1];

    if (i + 1 < this->faces.size()) {
      Vertex3 v2 = this->vertices[this->faces[i + 1] - 1];

      this->line(v1.x, v1.y, v2.x, v2.y, red);

      if (i + 2 < this->faces.size()) {
        Vertex3 v3 = this->vertices[this->faces[i + 2] - 1];

        this->line(v1.x, v1.y, v3.x, v3.y, red);
        this->line(v2.x, v2.y, v3.x, v3.y, red);
      }
    }
  }

  this->framebuffer.write_tga_file("framebuffer.tga");
}

#include "tgaimage.h"
#include <utility>

const TGAColor green = {0, 255, 0, 255};
const TGAColor red = {0, 0, 255, 255};

void triangle(int ax, int ay, int bx, int by, int cx, int cy,
              TGAImage &framebuffer, TGAColor color) {
  if (ay > by) {
    std::swap(ax, bx);
    std::swap(ay, by);
  }
  if (ay > cy) {
    std::swap(ax, cx);
    std::swap(ay, cy);
  }
  if (by > cy) {
    std::swap(bx, cx);
    std::swap(by, cy);
  }

  if (ay != by) {
    for (int y = ay; y < by; y++) {
      // These equations can be explain from the lesson Bresenham's line draw's
      // second attempt
      int x1 =
          std::round(ax + (cx - ax) * (y - ay) / static_cast<double>(cy - ay));
      int x2 =
          std::round(ax + (bx - ax) * (y - ay) / static_cast<double>(by - ay));

      for (int x = std::min(x1, x2); x < std::max(x1, x2); x++)
        framebuffer.set(x, y, color);
    }
  }

  if (by != cy) {
    for (int y = by; y < cy; y++) {
      int x1 =
          std::round(ax + (cx - ax) * (y - ay) / static_cast<double>(cy - ay));
      int x2 =
          std::round(bx + (cx - bx) * (y - by) / static_cast<double>(cy - by));

      for (int x = std::min(x1, x2); x < std::max(x1, x2); x++)
        framebuffer.set(x, y, color);
    }
  }
}

int main() {
  TGAImage framebuffer(600, 600, TGAImage::RGB);
  triangle(50, 50, 500, 100, 150, 300, framebuffer, red);

  framebuffer.write_tga_file("framebuffer.tga");
}

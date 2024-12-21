#include "tools.hh"
#include "canvas.hh"
#include <cmath>
#include <cstdlib>
#include <thread>
#include <vector>

Tool::Tool()
    : red(0), green(0), blue(0), alpha(0), radius(0), isActive(false),
      currentToolLeft(nullptr), currentToolRight(nullptr) {}

int Tool::mouseX = 0;
int Tool::mouseY = 0;

int Tool::prevMouseX = 0;
int Tool::prevMouseY = 0;
SDL_Renderer *Tool::renderer = nullptr;
Canvas *Tool::canvas = nullptr;

void Tool::putPixel(int x, int y) {
  int xIndex = (x >= canvas->windowWidth) ? canvas->windowWidth - 1 : x;
  int yIndex = (y >= canvas->windowHeight) ? canvas->windowHeight - 1 : y;

  canvas->canvasPixelGrid[xIndex][yIndex] =
      new Canvas::Pixel(red, green, 255, alpha);
  SDL_RenderDrawPoint(renderer, x, y);
};

void Tool::putPixel(int x, int y, int red, int green, int blue, int alpha) {
  int xIndex = (x >= canvas->windowWidth) ? canvas->windowWidth - 1 : x;
  int yIndex = (y >= canvas->windowHeight) ? canvas->windowHeight - 1 : y;

  canvas->canvasPixelGrid[xIndex][yIndex] =
      new Canvas::Pixel(red, green, blue, alpha);
  SDL_RenderDrawPoint(renderer, x, y);
}

void Paintbrush::circleDrawingAlgorithm(int xc, int yc, int circRadius) {

  for (int y = -circRadius; y <= circRadius; y++) {
    int xOffset = static_cast<int>(sqrt(circRadius * circRadius - y * y));

    // Calculate the left and right edges of the circle at this y level
    int x1 = xc - xOffset;
    int x2 = xc + xOffset;

    // Draw a horizontal line for the current y-coordinate
    for (int x = x1; x <= x2; x++) {

      int xIndex = (x >= canvas->windowWidth) ? canvas->windowWidth - 1 : x;
      int yIndex =
          (yc + y >= canvas->windowHeight) ? canvas->windowHeight - 1 : yc + y;

      if (canvas->canvasPixelGrid[xIndex][yIndex]->pixelStrokeLayer !=
          Canvas::Pixel::strokeLayer) {
          putPixel(x, yc + y);

      }
    }
  }
}

void Paintbrush::draw() {
  int tempx = prevMouseX;
  int tempy = prevMouseY;

  int dx = std::abs(mouseX - tempx);
  int dy = std::abs(mouseY - tempy);

  int stepx = (tempx < mouseX) ? 1 : -1;
  int stepy = (tempy < mouseY) ? 1 : -1;

  int err = dx - dy;

  SDL_SetRenderDrawColor(renderer, red, green, blue, alpha);

  circleDrawingAlgorithm(tempx, tempy, radius);
  while (tempx != mouseX || tempy != mouseY) {
    int e2 = 2 * err;

    if (e2 > -dy) {
      err -= dy;
      tempx += stepx;
    }
    if (e2 < dx) {
      err += dx;
      tempy += stepy;
    }

    circleDrawingAlgorithm(tempx, tempy, radius);
  }

  prevMouseX = mouseX;
  prevMouseY = mouseY;
}

void FloodFill::fill(int startX, int startY) {
  Canvas::Pixel *targetPixel = canvas->canvasPixelGrid[startX][startY];

  if (targetPixel->red == red && targetPixel->green == green &&
      targetPixel->blue == blue && targetPixel->alpha == alpha) {
    return;
  }
  int targetRed = targetPixel->red;
  int targetGreen = targetPixel->green;
  int targetBlue = targetPixel->blue;
  int targetAlpha = targetPixel->alpha;

  auto canFill = [&](int x, int y) {
    if (x < 0 || x >= canvas->windowWidth || y < 0 || y >= canvas->windowHeight)
      return false;

    Canvas::Pixel *pixel = canvas->canvasPixelGrid[x][y];
    if (!pixel)
      return false;

    return (pixel->red == targetRed && pixel->green == targetGreen &&
            pixel->blue == targetBlue && pixel->alpha == targetAlpha);
  };

  std::queue<std::pair<int, int>> pixelQueue;
  pixelQueue.push({startX, startY});

  SDL_SetRenderDrawColor(renderer, red, green, blue, alpha);

  while (!pixelQueue.empty()) {
    auto [x, y] = pixelQueue.front();
    pixelQueue.pop();

    if (!canFill(x, y))
      continue;

    putPixel(x, y, red, green, blue, alpha);

    pixelQueue.push({x + 1, y});
    pixelQueue.push({x - 1, y});
    pixelQueue.push({x, y + 1});
    pixelQueue.push({x, y - 1});
  }
}

void FloodFill::randomFill(int startX, int startY) {
  Canvas::Pixel *targetPixel = canvas->canvasPixelGrid[startX][startY];

  if (targetPixel->red == red && targetPixel->green == green &&
      targetPixel->blue == blue && targetPixel->alpha == alpha) {
    return;
  }
  int targetRed = targetPixel->red;
  int targetGreen = targetPixel->green;
  int targetBlue = targetPixel->blue;
  int targetAlpha = targetPixel->alpha;

  int fillRed = 0;
  int fillGreen = 100;
  int fillBlue = 0;

  auto canFill = [&](int x, int y) {
    if (x < 0 || x >= canvas->windowWidth || y < 0 || y >= canvas->windowHeight)
      return false;

    Canvas::Pixel *pixel = canvas->canvasPixelGrid[x][y];
    if (!pixel)
      return false;

    return (pixel->red == targetRed && pixel->green == targetGreen &&
            pixel->blue == targetBlue && pixel->alpha == targetAlpha);
  };

  std::queue<std::pair<int, int>> pixelQueue;
  pixelQueue.push({startX, startY});

  SDL_SetRenderDrawColor(renderer, red, green, blue, alpha);

  while (!pixelQueue.empty()) {
    auto [x, y] = pixelQueue.front();
    pixelQueue.pop();

    if (!canFill(x, y))
      continue;

    putPixel(x, y, rand(), rand(), rand(), 255);

    pixelQueue.push({x + 1, y});
    pixelQueue.push({x - 1, y});
    pixelQueue.push({x, y + 1});
    pixelQueue.push({x, y - 1});
  }
}
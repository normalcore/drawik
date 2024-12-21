#pragma once

#include "canvas.hh"
#include <SDL2/SDL.h>
#include <atomic>
#include <condition_variable>
#include <functional>
#include <iostream>
#include <mutex>
#include <queue>
#include <string>


class Tool {
public:
  Tool();

  static int mouseX;
  static int mouseY;

  static int prevMouseX;
  static int prevMouseY;

  unsigned int red;
  unsigned int green;
  unsigned int blue;
  unsigned int alpha;

  unsigned int radius;

  bool isActive;
  Tool *currentToolLeft;
  Tool *currentToolRight;

  static SDL_Renderer *renderer;
  static Canvas *canvas;
  void putPixel(int x, int y);
  void putPixel(int x, int y, int red, int green, int blue, int alpha);
};

class Paintbrush : public Tool {
public:
  Paintbrush() {}
  void draw();
  void circleDrawingAlgorithm(int xc, int yc, int circRadius);
};

class FloodFill : public Tool {
public:
  void fill(int startX, int startY);
  void randomFill(int startX, int startY);
};
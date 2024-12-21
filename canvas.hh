#pragma once

#include <SDL2/SDL.h>
#include <iostream>
#include <string>
#include <vector>

class Canvas {
public:
  Canvas(unsigned int windowPosX, unsigned int windowPosY,
         unsigned int windowWidth, unsigned int windowHeight,
         SDL_WindowFlags windowFlag, SDL_RendererFlags rendererFlag);
  ~Canvas();
  void flushCanvas();
  void drawPixelGrid();
  void setBGColor(unsigned int r = 255, unsigned int g = 255,
                  unsigned int b = 255, unsigned int a = 255);
  SDL_Window *window;
  SDL_Renderer *renderer;
  struct Pixel {
    Pixel(unsigned int red, unsigned int green, unsigned int blue,
          unsigned int alpha)
        : red(red), green(green), blue(blue), alpha(alpha) {
      pixelStrokeLayer = Pixel::strokeLayer;
    }
    Pixel(void) : red(0), green(0), blue(0), alpha(0) {
      pixelStrokeLayer = Pixel::strokeLayer;
    }
    bool operator==(Pixel x) {
      return red == x.red && green == x.green && blue == x.blue && alpha == x.alpha;
    }

    unsigned int red;
    unsigned int green;
    unsigned int blue;
    unsigned int alpha;
    static unsigned int strokeLayer;
    unsigned int pixelStrokeLayer;
  };
  Pixel ***canvasPixelGrid;
  bool isRunning;
  unsigned int windowWidth;
  unsigned int windowHeight;

private:
  unsigned int bgRed;
  unsigned int bgGreen;
  unsigned int bgBlue;
  unsigned int bgAlpha;
  unsigned int windowPosX;
  unsigned int windowPosY;
  SDL_WindowFlags windowFlag;
  SDL_RendererFlags rendererFlag;
};
#include "canvas.hh"

unsigned int Canvas::Pixel::strokeLayer = 0;

void Canvas::setBGColor(unsigned int r, unsigned int g, unsigned int b,
                        unsigned int a) {
  bgRed = r;
  bgGreen = g;
  bgBlue = b;
  bgAlpha = a;
}

Canvas::Canvas(unsigned int windowPosX, unsigned int windowPosY,
               unsigned int windowWidth, unsigned int windowHeight,
               SDL_WindowFlags windowFlag, SDL_RendererFlags rendererFlag)
    : windowPosX(windowPosX), windowPosY(windowPosY), windowWidth(windowWidth),
      windowHeight(windowHeight) {

  window = SDL_CreateWindow("Drawik", windowPosX, windowPosY, windowWidth,
                            windowHeight, windowFlag);
  renderer = SDL_CreateRenderer(window, -1, rendererFlag);
  SDL_Init(SDL_INIT_VIDEO);
  canvasPixelGrid = new Pixel **[windowWidth];

  setBGColor();
  for (int i = 0; i < windowWidth; ++i) {
    canvasPixelGrid[i] = new Pixel *[windowHeight];

    for (int j = 0; j < windowHeight; ++j) {
      canvasPixelGrid[i][j] = new Pixel(bgRed, bgGreen, bgBlue, bgAlpha);
    }
  }

  isRunning = true;
}

void Canvas::drawPixelGrid() {
  for (int i = 0; i < windowWidth; ++i) {
    for (int j = 0; j < windowHeight; ++j) {
      SDL_SetRenderDrawColor(
          renderer, canvasPixelGrid[i][j]->red, canvasPixelGrid[i][j]->green,
          canvasPixelGrid[i][j]->blue, canvasPixelGrid[i][j]->alpha);
      SDL_RenderDrawPoint(renderer, i, j);
    }
  }
}

Canvas::~Canvas() {
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
}

void Canvas::flushCanvas() {
  for (int i = 0; i < windowWidth; ++i) {
    for (int j = 0; j < windowHeight; ++j) {
      canvasPixelGrid[i][j]->red = bgRed;
      canvasPixelGrid[i][j]->green = bgGreen;
      canvasPixelGrid[i][j]->blue = bgBlue;
      canvasPixelGrid[i][j]->alpha = bgAlpha;
    }
  }

  SDL_SetRenderDrawColor(renderer, bgRed, bgGreen, bgBlue, bgAlpha);
  SDL_RenderClear(renderer);
}
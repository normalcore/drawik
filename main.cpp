#include <SDL2/SDL.h>
#include <iostream>
#include <string>

#include "canvas.hh"
#include "tools.hh"

int main() {
  Canvas canvas(0, 0, 1920, 1080, SDL_WINDOW_FULLSCREEN,
                SDL_RENDERER_ACCELERATED);
  Tool tool;
  tool.renderer = canvas.renderer;
  tool.canvas = &canvas;

  Paintbrush brush(std::thread::hardware_concurrency());
  brush.radius = 7;

  FloodFill fill;

  canvas.flushCanvas();

  // Add frame limiting
  const int FPS = 60;
  const int FRAME_DELAY = 1000 / FPS;

  Uint32 frameStart;
  int frameTime;
  SDL_Event event;
  
  while (canvas.isRunning) {
    frameStart = SDL_GetTicks();
    SDL_GetMouseState(&tool.mouseX, &tool.mouseY);

    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_QUIT) {
        canvas.isRunning = false;
      }
      if (event.type == SDL_MOUSEBUTTONDOWN &&
          event.button.button == SDL_BUTTON_LEFT) {
        SDL_GetMouseState(&tool.prevMouseX, &tool.prevMouseY);
        Canvas::Pixel::strokeLayer++;
        brush.isActive = true;
      }
      if (event.type == SDL_MOUSEBUTTONUP &&
          event.button.button == SDL_BUTTON_LEFT) {
        brush.isActive = false;
        canvas.drawPixelGrid();
      }
      if (event.type == SDL_MOUSEBUTTONDOWN &&
          event.button.button == SDL_BUTTON_RIGHT) {
        fill.randomFill(tool.mouseX, tool.mouseY);
        canvas.drawPixelGrid();
      }
      if (event.type == SDL_KEYDOWN) {
        if (event.key.keysym.scancode == SDL_SCANCODE_ESCAPE) {

          canvas.flushCanvas();
        }
      }
    }

    if (brush.isActive) {

      brush.draw();
    }
    SDL_RenderPresent(canvas.renderer);
    
    // Your existing event handling and drawing code here   
    frameTime = SDL_GetTicks() - frameStart;
    if (FRAME_DELAY > frameTime) {
        SDL_Delay(FRAME_DELAY - frameTime);
    }
  }

  return 0;
}
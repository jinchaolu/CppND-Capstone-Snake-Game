#ifndef RENDERER_H
#define RENDERER_H

#include <vector>
#include "SDL.h"
#include "snake.h"

class Renderer {
 public:
  Renderer(const std::size_t screen_width, const std::size_t screen_height,
           const std::size_t grid_width, const std::size_t grid_height);
  ~Renderer();

  void Render(Snake const snake, SDL_Point const &food, bool foodVisible = true);
  void UpdateWindowTitle(int score, int fps);
  SDL_Window* GetWindow() const { return sdl_window; }
  SDL_Renderer* GetRenderer() const { return sdl_renderer; }
  std::size_t GetScreenWidth() const { return screen_width; }
  std::size_t GetScreenHeight() const { return screen_height; }

 private:
  SDL_Window *sdl_window;
  SDL_Renderer *sdl_renderer;

  const std::size_t screen_width;
  const std::size_t screen_height;
  const std::size_t grid_width;
  const std::size_t grid_height;
};

#endif
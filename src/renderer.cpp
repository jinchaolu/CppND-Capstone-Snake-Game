#include "renderer.h"
#include <iostream>
#include <string>

Renderer::Renderer(const std::size_t screen_width,
                   const std::size_t screen_height,
                   const std::size_t grid_width, const std::size_t grid_height)
    : screen_width(screen_width),
      screen_height(screen_height),
      grid_width(grid_width),
      grid_height(grid_height) {
  // Initialize SDL
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    std::cerr << "SDL could not initialize.\n";
    std::cerr << "SDL_Error: " << SDL_GetError() << "\n";
  }

  // Initialize SDL_ttf
  if (TTF_Init() == -1) {
    std::cerr << "SDL_ttf could not initialize.\n";
    std::cerr << "SDL_ttf Error: " << TTF_GetError() << "\n";
  }

  // Load font
  font = TTF_OpenFont("/System/Library/Fonts/Helvetica.ttc", 24);
  if (font == nullptr) {
    std::cerr << "Failed to load font.\n";
    std::cerr << "SDL_ttf Error: " << TTF_GetError() << "\n";
  }

  // Create Window
  sdl_window = SDL_CreateWindow("Snake Game", SDL_WINDOWPOS_CENTERED,
                                SDL_WINDOWPOS_CENTERED, screen_width,
                                screen_height, SDL_WINDOW_SHOWN);

  if (nullptr == sdl_window) {
    std::cerr << "Window could not be created.\n";
    std::cerr << " SDL_Error: " << SDL_GetError() << "\n";
  }

  // Create renderer
  sdl_renderer = SDL_CreateRenderer(sdl_window, -1, SDL_RENDERER_ACCELERATED);
  if (nullptr == sdl_renderer) {
    std::cerr << "Renderer could not be created.\n";
    std::cerr << "SDL_Error: " << SDL_GetError() << "\n";
  }
}

Renderer::~Renderer() {
  TTF_CloseFont(font);
  TTF_Quit();
  SDL_DestroyWindow(sdl_window);
  SDL_Quit();
}

void Renderer::RenderText(const std::string &text, int x, int y, SDL_Color color) {
  SDL_Surface* surface = TTF_RenderText_Solid(font, text.c_str(), color);
  if (surface == nullptr) {
    std::cerr << "Unable to render text surface! SDL_ttf Error: " << TTF_GetError() << "\n";
    return;
  }

  SDL_Texture* texture = SDL_CreateTextureFromSurface(sdl_renderer, surface);
  if (texture == nullptr) {
    std::cerr << "Unable to create texture from rendered text! SDL Error: " << SDL_GetError() << "\n";
    SDL_FreeSurface(surface);
    return;
  }

  SDL_Rect renderQuad = {x, y, surface->w, surface->h};
  SDL_RenderCopy(sdl_renderer, texture, NULL, &renderQuad);

  SDL_FreeSurface(surface);
  SDL_DestroyTexture(texture);
}

void Renderer::RenderMenu(const std::vector<std::string> &menu_items) {
  SDL_SetRenderDrawColor(sdl_renderer, 0x1E, 0x1E, 0x1E, 0xFF);
  SDL_RenderClear(sdl_renderer);

  SDL_Color textColor = {255, 255, 255, 255};  // White
  int y_pos = 100;

  // Render title
  RenderText("=== Snake Game Menu ===", screen_width/4, y_pos, textColor);
  y_pos += 50;

  // Render menu items
  for (const auto &item : menu_items) {
    RenderText(item, screen_width/4, y_pos, textColor);
    y_pos += 40;
  }

  SDL_RenderPresent(sdl_renderer);
}

void Renderer::Render(Snake const snake, SDL_Point const &food) {
  SDL_Rect block;
  block.w = screen_width / grid_width;
  block.h = screen_height / grid_height;

  // Clear screen
  SDL_SetRenderDrawColor(sdl_renderer, 0x1E, 0x1E, 0x1E, 0xFF);
  SDL_RenderClear(sdl_renderer);

  // Render food
  SDL_SetRenderDrawColor(sdl_renderer, 0xFF, 0xCC, 0x00, 0xFF);
  block.x = food.x * block.w;
  block.y = food.y * block.h;
  SDL_RenderFillRect(sdl_renderer, &block);

  // Render snake's body
  SDL_SetRenderDrawColor(sdl_renderer, 0xFF, 0xFF, 0xFF, 0xFF);
  for (SDL_Point const &point : snake.body) {
    block.x = point.x * block.w;
    block.y = point.y * block.h;
    SDL_RenderFillRect(sdl_renderer, &block);
  }

  // Render snake's head
  block.x = static_cast<int>(snake.head_x) * block.w;
  block.y = static_cast<int>(snake.head_y) * block.h;
  if (snake.alive) {
    SDL_SetRenderDrawColor(sdl_renderer, 0x00, 0x7A, 0xCC, 0xFF);
  } else {
    SDL_SetRenderDrawColor(sdl_renderer, 0xFF, 0x00, 0x00, 0xFF);
  }
  SDL_RenderFillRect(sdl_renderer, &block);

  // Update Screen
  SDL_RenderPresent(sdl_renderer);
}

void Renderer::UpdateWindowTitle(int score, int fps, bool isPaused) {
  std::string title{"Snake Score: " + std::to_string(score) + " FPS: " + std::to_string(fps)};
  // Add message to tell the user game is paused
  if (isPaused) {
    title += " (Paused)";
  }
  SDL_SetWindowTitle(sdl_window, title.c_str());
}

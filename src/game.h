#ifndef GAME_H
#define GAME_H

#include <random>
#include "SDL.h"
#include "controller.h"
#include "renderer.h"
#include "snake.h"

class Controller; // Forward declaration

class Game {
 public:
  enum class GameState { Menu, Playing, GameOver };
  void Reset();
  void ShowMenu();
  void DisplayLeaderboard();
  Game(std::size_t grid_width, std::size_t grid_height);
  void Run(Controller const &controller, Renderer &renderer,
           std::size_t target_frame_duration);
  int GetScore() const;
  int GetSize() const;
  bool GetPause() const;
  void PauseGame();
  void ResumeGame();
  void SetRenderer(Renderer* r) { renderer = r; }

  struct LeaderboardEntry {
    std::string name;
    int score;
    std::string date;
    std::string time;
  };
  std::vector<LeaderboardEntry> leaderboard;
  void LoadLeaderboard();
  void SaveLeaderboard();
  bool IsTopScore(int score);
  std::string PromptName();
  void CheckAndUpdateLeaderboard();

 private:
  Snake snake;
  SDL_Point food;

  std::random_device dev;
  std::mt19937 engine;
  std::uniform_int_distribution<int> random_w;
  std::uniform_int_distribution<int> random_h;

  int score{0};

  void PlaceFood();
  void Update();
  bool isPaused{false};
  GameState state{GameState::Menu};
  void HandleMenuInput();
};

#endif
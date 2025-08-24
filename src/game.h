#ifndef GAME_H
#define GAME_H

#include <random>
#include <memory>
#include "SDL.h"
#include "controller.h"
#include "renderer.h"
#include "snake.h"
#include "state.h"

class Controller;
class State;

class Game {
 public:
  Game(std::size_t grid_width, std::size_t grid_height);
  void Run(Controller const &controller, Renderer &renderer,
           std::size_t target_frame_duration);
  
  // State management
  void Reset();
  void ChangeState(std::unique_ptr<State> newState);
  
  // Menu functions
  void ShowMenu();
  void HandleMenuInput();
  void DisplayLeaderboard();
  
  // Game state access
  int GetScore() const { return score; }
  int GetSize() const { return snake.size; }
  bool GetPause() const { return isPaused; }
  void PauseGame() { isPaused = true; }
  void ResumeGame() { isPaused = false; }
  void Update();
  
  // Leaderboard management
  std::vector<LeaderboardEntry> leaderboard;
  void LoadLeaderboard();
  void SaveLeaderboard();
  bool IsTopScore(int score);
  std::string PromptName();
  void CheckAndUpdateLeaderboard();
  
  // Getters for state pattern
  Snake& GetSnake() { return snake; }
  SDL_Point& GetFood() { return food; }
  void PlaceFood();

 private:
  Snake snake;
  SDL_Point food;
  std::unique_ptr<State> currentState;
  std::random_device dev;
  std::mt19937 engine;
  std::uniform_int_distribution<int> random_w;
  std::uniform_int_distribution<int> random_h;

  int score{0};
  bool isPaused{false};
  Renderer* renderer{nullptr};
};

#endif
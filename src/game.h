#ifndef GAME_H
#define GAME_H

#include <random>
#include <memory>
#include "SDL.h"
#include "controller.h"
#include "renderer.h"
#include "snake.h"

class Controller;
class State;  // Forward declaration for the new State base class

class Game {
 public:
  Game(std::size_t grid_width, std::size_t grid_height);
  void Run(Controller const &controller, Renderer &renderer,
           std::size_t target_frame_duration);
  
  // Existing methods
  void Reset();
  int GetScore() const { return score; }
  int GetSize() const { return snake.size; }
  bool GetPause() const { return isPaused; }
  void PauseGame() { isPaused = true; }
  void ResumeGame() { isPaused = false; }
  void SetRenderer(Renderer* r) { renderer = r; }

  // Make these public so states can access them
  Snake& GetSnake() { return snake; }
  SDL_Point& GetFood() { return food; }
  void PlaceFood();

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
  
  // New method for state management
  void ChangeState(std::unique_ptr<State> newState);

 private:
  Snake snake;
  SDL_Point food;
  std::unique_ptr<State> currentState;  // New member for current state

  std::random_device dev;
  std::mt19937 engine;
  std::uniform_int_distribution<int> random_w;
  std::uniform_int_distribution<int> random_h;

  int score{0};
  bool isPaused{false};
  Renderer* renderer{nullptr};
};

#endif
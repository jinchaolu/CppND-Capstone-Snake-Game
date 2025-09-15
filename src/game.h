#ifndef GAME_H
#define GAME_H

#include <random>
#include <memory>
#include <vector>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <future>
#include <atomic>
#include "SDL.h"
#include "controller.h"
#include "renderer.h"
#include "snake.h"
#include "state.h"

class Controller;
class State;

enum class Difficulty {
    Beginner,
    Normal,
    Advanced,
    Expert
};

struct DifficultyConfig {
    float baseSpeed;
    int scorePerFood;
    int growthRate;
    float speedIncrease;
    bool hasFoodTimer;
    int foodTimeLimit;
};

class Game {
 public:
  struct LeaderboardEntry {
    std::string name;
    int score;
    std::string date;
    std::string time;
    Difficulty difficulty;
  };

  Game(std::size_t grid_width, std::size_t grid_height);
  ~Game();
  void Run(Controller const &controller, Renderer &renderer,
           std::size_t target_frame_duration);
  
  // State management
  void Reset();
  void ChangeState(std::unique_ptr<State> newState);
  
  // Menu functions
  void ShowMenu();
  void HandleMenuInput();
  void DisplayLeaderboard();
  void Update();
  
  // Getters/Setters
  int GetScore() const { return score; }
  int GetSize() const { return snake.size; }
  bool GetPause() const { return isPaused; }
  void PauseGame();
  void ResumeGame();
  Snake& GetSnake() { return snake; }
  SDL_Point& GetFood() { return food; }
  void SetRenderer(Renderer* r) { renderer = r; }
  
  // Difficulty management
  void SetDifficulty(Difficulty diff);
  Difficulty GetDifficulty() const { return currentDifficulty; }
  const DifficultyConfig& GetConfig() const { return diffConfig; }
  
  // Leaderboard management
  std::vector<LeaderboardEntry> leaderboard;
  void LoadLeaderboard();
  void SaveLeaderboard();
  bool IsTopScore(int score);
  std::string PromptName();
  void CheckAndUpdateLeaderboard();
  void PlaceFood();

  // Concurrency methods
  void StartBackgroundTasks();
  void StopBackgroundTasks();
  void WaitForLeaderboardLoad();

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

  // Difficulty settings
  Difficulty currentDifficulty{Difficulty::Normal};
  DifficultyConfig diffConfig;
  void InitializeDifficultyConfig();
  float foodTimer{0.0f};

  // Concurrency members
  std::thread foodTimerThread;
  std::thread leaderboardThread;
  std::mutex scoreMutex;
  std::mutex pauseMutex;
  std::condition_variable pauseCV;
  std::promise<bool> leaderboardPromise;
  std::future<bool> leaderboardFuture;
  std::atomic<bool> shouldStop{false};
  std::atomic<bool> foodExpired{false};
  
  // Background task methods
  void FoodTimerTask();
  void LoadLeaderboardAsync();
  void UpdateScoreThreadSafe(int points);
};

#endif
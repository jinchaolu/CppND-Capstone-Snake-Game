#include "game.h"
#include "state.h"
#include <algorithm>
#include <chrono>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <regex>
#include <string>
#include <sys/stat.h>
#include <unistd.h>

Game::Game(std::size_t grid_width, std::size_t grid_height)
    : snake(grid_width, grid_height),
      engine(dev()),
      random_w(0, static_cast<int>(grid_width - 1)),
      random_h(0, static_cast<int>(grid_height - 1)) {
  PlaceFood();
  InitializeDifficultyConfig();
  currentState = std::make_unique<MenuState>();
}

void Game::InitializeDifficultyConfig() {
    switch (currentDifficulty) {
        case Difficulty::Beginner:
            diffConfig = {0.1f, 1, 1, 0.01f, false, 0};
            break;
        case Difficulty::Normal:
            diffConfig = {0.15f, 2, 2, 0.02f, false, 0};
            break;
        case Difficulty::Advanced:
            diffConfig = {0.2f, 3, 3, 0.03f, true, 10};
            break;
        case Difficulty::Expert:
            diffConfig = {0.25f, 5, 4, 0.04f, true, 7};
            break;
    }
    snake.speed = diffConfig.baseSpeed;
}

void Game::SetDifficulty(Difficulty diff) {
    currentDifficulty = diff;
    InitializeDifficultyConfig();
}

void Game::ShowMenu() {
  std::cout << "\n=== Snake Game Menu ===\n";
  std::cout << "1. New Game (Press N)\n";
  std::cout << "2. Leaderboard (Press L)\n";
  std::cout << "3. Exit (Press Q)\n";
  
  // Set window title for menu state
  if (renderer) {
    SDL_SetWindowTitle(SDL_GetWindowFromID(1), "Snake Game - Press N:New Game, L:Leaderboard, Q:Quit");
  }
}

void Game::DisplayLeaderboard() {
  LoadLeaderboard();
  std::cout << "\n=== Leaderboard ===\n";
  if (leaderboard.empty()) {
    std::cout << "No scores yet!\n";
  } else {
    for (size_t i = 0; i < leaderboard.size(); ++i) {
      std::cout << i+1 << ". " << leaderboard[i].name 
                << " - Score: " << leaderboard[i].score 
                << " (" << leaderboard[i].date << " " << leaderboard[i].time << ")\n";
    }
  }
  std::cout << "\nPress any key to continue...\n";  // Updated message
}

void Game::HandleMenuInput() {
  SDL_Event e;
  bool choice_made = false;
  while (!choice_made && SDL_WaitEvent(&e)) {
    if (e.type == SDL_QUIT) {
      snake.alive = false;  // This will exit the game
      return;
    }
    if (e.type == SDL_KEYDOWN) {
      switch (e.key.keysym.sym) {
        case SDLK_n:  // New Game
          Reset();
          currentState = std::make_unique<PlayingState>();
          choice_made = true;
          break;
        case SDLK_l:  // Leaderboard
          DisplayLeaderboard();
          break;
        case SDLK_q:  // Quit
          snake.alive = false;
          choice_made = true;
          return;
        case SDLK_m:  // Return to menu from leaderboard
          ShowMenu();
          break;
      }
    }
  }
}

void Game::Run(Controller const &controller, Renderer &renderer,
               std::size_t target_frame_duration) {
  Uint32 title_timestamp = SDL_GetTicks();
  Uint32 frame_start;
  Uint32 frame_end;
  Uint32 frame_duration;
  int frame_count = 0;
  bool running = true;

  SetRenderer(&renderer);

  while (running && snake.alive) {
    frame_start = SDL_GetTicks();

    // Input
    currentState->HandleInput(*this, controller);

    // Update
    currentState->Update(*this);

    // Render
    currentState->Render(*this, renderer);

    frame_end = SDL_GetTicks();

    // Keep track of how long each loop through the input/update/render cycle takes
    frame_count++;
    frame_duration = frame_end - frame_start;

    // After every second, update the window title
    if (frame_end - title_timestamp >= 1000) {
      renderer.UpdateWindowTitle(score, frame_count);
      frame_count = 0;
      title_timestamp = frame_end;
    }

    // If the time for this frame is too small (i.e. frame_duration is
    // smaller than the target ms_per_frame), delay the loop to
    // achieve the correct frame rate.
    if (frame_duration < target_frame_duration) {
      SDL_Delay(target_frame_duration - frame_duration);
    }
  }
}

// --- Leaderboard Management ---
void Game::LoadLeaderboard() {
  leaderboard.clear();
  
  mkdir("data", 0777);  // Create data directory if it doesn't exist, with full permissions
  
  // Try to open the leaderboard file
  std::ifstream file("data/Leaderboard.txt");
  if (!file.is_open()) {
    // If file doesn't exist, create it with default entries
    std::ofstream newFile("data/Leaderboard.txt");
    if (newFile.is_open()) {
      newFile << "Name,Score,Date,Time\n";
      // Add 10 default entries
      for (int i = 1; i <= 10; i++) {
        newFile << "player" << i << ",0,2025-01-01,00:00:00\n";
      }
      newFile.close();
      // Reopen file for reading
      file.open("data/Leaderboard.txt");
    }
  }
  
  if (!file.is_open()) {
    std::cerr << "Failed to open or create leaderboard file\n";
    return;
  }
  
  std::string line;
  std::getline(file, line); // skip header
  while (std::getline(file, line)) {
    size_t p1 = line.find(',');
    size_t p2 = line.find(',', p1+1);
    size_t p3 = line.find(',', p2+1);
    if (p1 == std::string::npos || p2 == std::string::npos || p3 == std::string::npos) {
      std::cerr << "Invalid line format: " << line << "\n";
      continue;
    }
    LeaderboardEntry entry;
    entry.name = line.substr(0, p1);
    entry.score = std::stoi(line.substr(p1+1, p2-p1-1));
    entry.date = line.substr(p2+1, p3-p2-1);
    entry.time = line.substr(p3+1);
    leaderboard.push_back(entry);
  }
}

void Game::SaveLeaderboard() {
  mkdir("data", 0777);  // Create data directory if it doesn't exist, with full permissions
  std::ofstream file("data/Leaderboard.txt");
  if (!file.is_open()) {
    std::cerr << "Error: Could not open leaderboard file for writing\n";
    // Try to ensure the directory exists
    system("mkdir -p data");
    file.open("data/Leaderboard.txt");
    if (!file.is_open()) {
      std::cerr << "Error: Still could not open leaderboard file after creating directory\n";
      return;
    }
  }
  file << "Name,Score,Date,Time\n";
  for (const auto& entry : leaderboard) {
    file << entry.name << "," << entry.score << "," << entry.date << "," << entry.time << "\n";
  }
}

bool Game::IsTopScore(int s) {
  if (leaderboard.size() < 10) return true;
  for (const auto& entry : leaderboard) {
    if (s > entry.score) return true;
  }
  return false;
}

std::string Game::PromptName() {
  std::string name;
  std::regex valid("^[A-Za-z0-9_]{1,8}$");
  while (true) {
    std::cout << "Congratulations! You made the top 10! Enter your name (max 8 chars, letters/numbers/_): ";
    std::getline(std::cin, name);
    if (std::regex_match(name, valid)) break;
    std::cout << "Invalid name. Try again.\n";
  }
  return name;
}

void Game::CheckAndUpdateLeaderboard() {
  LoadLeaderboard();
  if (!IsTopScore(score)) return;
  std::string name = PromptName();
  // Get current date/time
  std::time_t t = std::time(nullptr);
  std::tm* now = std::localtime(&t);
  char datebuf[11], timebuf[9];
  std::strftime(datebuf, sizeof(datebuf), "%Y-%m-%d", now);
  std::strftime(timebuf, sizeof(timebuf), "%H:%M:%S", now);
  LeaderboardEntry newEntry{name, score, datebuf, timebuf};
  leaderboard.push_back(newEntry);
  std::sort(leaderboard.begin(), leaderboard.end(), [](const auto& a, const auto& b){ return a.score > b.score; });
  if (leaderboard.size() > 10) leaderboard.resize(10);
  SaveLeaderboard();
  std::cout << "Leaderboard updated!\n";
}

void Game::PlaceFood() {
  int x, y;
  while (true) {
    x = random_w(engine);
    y = random_h(engine);
    // Check that the location is not occupied by a snake item before placing
    // food.
    if (!snake.SnakeCell(x, y)) {
      food.x = x;
      food.y = y;
      return;
    }
  }
}

void Game::Update() {
  if (!snake.alive) return;
  if (isPaused) return;

  snake.Update();

  int new_x = static_cast<int>(snake.head_x);
  int new_y = static_cast<int>(snake.head_y);

  // Check if there's food
  if (food.x == new_x && food.y == new_y) {
    score += diffConfig.scorePerFood;
    
    // Call GrowBody() multiple times based on difficulty
    for (int i = 0; i < diffConfig.growthRate; i++) {
      snake.GrowBody();
    }
    
    snake.speed += diffConfig.speedIncrease;
    PlaceFood();
  }
}

void Game::Reset() {
  score = 0;
  snake.alive = true;
  snake.size = 1;
  snake.head_x = random_w.max() / 2;
  snake.head_y = random_h.max() / 2;
  snake.direction = Snake::Direction::kUp;
  snake.body.clear();
  snake.speed = diffConfig.baseSpeed;
  PlaceFood();
  isPaused = false;
}

void Game::ChangeState(std::unique_ptr<State> newState) {
  currentState = std::move(newState);
}
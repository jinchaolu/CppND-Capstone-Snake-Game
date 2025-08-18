#include <algorithm>
#include <regex>
#include <fstream>
#include <string>
#include "game.h"
#include <iostream>
#include "SDL.h"

Game::Game(std::size_t grid_width, std::size_t grid_height)
    : snake(grid_width, grid_height),
      engine(dev()),
      random_w(0, static_cast<int>(grid_width - 1)),
      random_h(0, static_cast<int>(grid_height - 1)) {
  PlaceFood();
}

void Game::Run(Controller const &controller, Renderer &renderer,
               std::size_t target_frame_duration) {
  Uint32 title_timestamp = SDL_GetTicks();
  Uint32 frame_start;
  Uint32 frame_end;
  Uint32 frame_duration;
  int frame_count = 0;
  bool running = true;

  while (running) {
    frame_start = SDL_GetTicks();

    // Input, Update, Render - the main game loop.
    controller.HandleInput(running, snake, *this);
    // Update snake if it is not paused
    if (!isPaused) {
      Update();
    }
    renderer.Render(snake, food);

    // If snake died, check leaderboard and break loop
    if (!snake.alive) {
      CheckAndUpdateLeaderboard();
      break;
    }

    frame_end = SDL_GetTicks();

    // Keep track of how long each loop through the input/update/render cycle
    // takes.
    frame_count++;
    frame_duration = frame_end - frame_start;

    // After every second, update the window title.
    if (frame_end - title_timestamp >= 1000) {
      renderer.UpdateWindowTitle(score, frame_count, isPaused);
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
  std::ifstream file("data/Leaderboard.txt");
  std::string line;
  std::getline(file, line); // skip header
  while (std::getline(file, line)) {
    size_t p1 = line.find(',');
    size_t p2 = line.find(',', p1+1);
    size_t p3 = line.find(',', p2+1);
    if (p1 == std::string::npos || p2 == std::string::npos || p3 == std::string::npos) continue;
    LeaderboardEntry entry;
    entry.name = line.substr(0, p1);
    entry.score = std::stoi(line.substr(p1+1, p2-p1-1));
    entry.date = line.substr(p2+1, p3-p2-1);
    entry.time = line.substr(p3+1);
    leaderboard.push_back(entry);
  }
}

void Game::SaveLeaderboard() {
  std::ofstream file("data/Leaderboard.txt");
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

  snake.Update();

  int new_x = static_cast<int>(snake.head_x);
  int new_y = static_cast<int>(snake.head_y);

  // Check if there's food over here
  if (food.x == new_x && food.y == new_y) {
    score++;
    PlaceFood();
    // Grow snake and increase speed.
    snake.GrowBody();
    snake.speed += 0.02;
  }
}

int Game::GetScore() const { return score; }
int Game::GetSize() const { return snake.size; }
bool Game::GetPause() const { return isPaused; }
void Game::PauseGame() { isPaused = true; }
void Game::ResumeGame() { isPaused = false; }
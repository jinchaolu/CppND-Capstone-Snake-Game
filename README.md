# CPPND: Capstone Snake Game Example

This is a starter repo for the Capstone project in the [Udacity C++ Nanodegree Program](https://www.udacity.com/course/c-plus-plus-nanodegree--nd213). The code for this repo was inspired by [this](https://codereview.stackexchange.com/questions/212296/snake-game-in-c-with-sdl) excellent StackOverflow post and set of responses.

<img src="snake_game.gif"/>

The Capstone Project gives you a chance to integrate what you've learned throughout this program. This project will become an important part of your portfolio to share with current and future colleagues and employers.

In this project, you can build your own C++ application or extend this Snake game, following the principles you have learned throughout this Nanodegree Program. This project will demonstrate that you can independently create applications using a wide range of C++ features.

## Dependencies for Running Locally
* cmake >= 3.7
  * All OSes: [click here for installation instructions](https://cmake.org/install/)
* make >= 4.1 (Linux, Mac), 3.81 (Windows)
  * Linux: make is installed by default on most Linux distros
  * Mac: [install Xcode command line tools to get make](https://developer.apple.com/xcode/features/)
  * Windows: [Click here for installation instructions](http://gnuwin32.sourceforge.net/packages/make.htm)
* SDL2 >= 2.0
  * All installation instructions can be found [here](https://wiki.libsdl.org/Installation)
  >Note that for Linux, an `apt` or `apt-get` installation is preferred to building from source. 
* gcc/g++ >= 5.4
  * Linux: gcc / g++ is installed by default on most Linux distros
  * Mac: same deal as make - [install Xcode command line tools](https://developer.apple.com/xcode/features/)
  * Windows: recommend using [MinGW](http://www.mingw.org/)

## Basic Build Instructions

1. Clone this repo.
2. Make a build directory in the top level directory: `mkdir build && cd build`
3. Compile: `cmake .. && make`
4. Run it: `./SnakeGame`.


## CC Attribution-ShareAlike 4.0 International


Shield: [![CC BY-SA 4.0][cc-by-sa-shield]][cc-by-sa]

This work is licensed under a
[Creative Commons Attribution-ShareAlike 4.0 International License][cc-by-sa].

[![CC BY-SA 4.0][cc-by-sa-image]][cc-by-sa]

[cc-by-sa]: http://creativecommons.org/licenses/by-sa/4.0/
[cc-by-sa-image]: https://licensebuttons.net/l/by-sa/4.0/88x31.png
[cc-by-sa-shield]: https://img.shields.io/badge/License-CC%20BY--SA%204.0-lightgrey.svg

## (TODO)New Features
### 1. Pause
Game should be able to be paused and resumed by pressing `space` button.  
### 2. (Proposed)Enter game time at the start of the game
### 3. (Proposed)Display remaining time
### 4. (Proposed)Display elapsed time
### 5. (Proposed)Choose difficulty at the start of the game
Player should be able to select difficulty at the start of the game.

## (TODO)Project Rubric  
### 1. README (All Rubric Points REQUIRED)
#### 1.1 A README with instructions is included with the project
Yes, it is.
#### 1.2 The README indicates the new features you added to the game.
Yes, it is.
#### 1.3 The README includes information about each rubric point addressed.
Yes, it is.
### 2. Compiling and Testing (All Rubric Points REQUIRED)
#### 2.1 The submission must compile and run without errors on the Udacity project workspace.
Yes, it is.
### 3. Loops, Functions, I/O - meet at least 2 criteria
#### 3.1 The project demonstrates an understanding of C++ functions and control structures.
Yes, it is.
#### 3.2 The project reads data from a file and process the data, or the program writes data to a file.
Yes, it is. The game maintains a leaderboard in `data/Leaderboard.txt`, reading and writing the top 10 scores as a CSV file. When the game ends and the player's score is in the top 10, the leaderboard is updated and saved, always keeping the top 10 scores with date and time.
#### 3.3 The project accepts user input and processes the input.
Yes, it is. When a new high score is achieved, the user is prompted to enter their name (max 8 chars, only letters, numbers, or underscore) for the leaderboard, with input validation.
#### 3.4 The project uses data structures and immutable variables.
Yes, it is. The leaderboard is managed using a vector of structs, demonstrating the use of C++ STL containers and custom data types.
### 4. Object Oriented Programming - meet at least 3 criteria
#### 4.1 One or more classes are added to the project with appropriate access specifiers for class members.
Yes. Added State class hierarchy with PlayingState, MenuState, and GameOverState classes. Each class has appropriate public interfaces and protected/private implementation details.

#### 4.2 Class constructors utilize member initialization lists.
Yes. State classes use initialization lists for member variables (e.g., `MenuState() : menuShown(false) {}` and `PlayingState() : lastPauseState(false) {}`).

#### 4.3 Classes abstract implementation details from their interfaces.
Yes. The State pattern implementation hides game state management details behind clean interfaces. Each state handles its own rendering, input processing, and update logic while exposing only the necessary public methods (Update, Render, HandleInput).

#### 4.4 Overloaded functions allow the same function to operate on different parameters.
Yes, it is.

#### 4.5 Classes follow an appropriate inheritance hierarchy with virtual and override functions.
Yes. Implemented State pattern using inheritance:
- Abstract base class `State` with virtual functions
- Derived classes (PlayingState, MenuState, GameOverState) that override these functions
- Each state handles its specific behavior while maintaining a common interface
- Virtual destructor ensures proper cleanup

#### 4.6 Templates generalize functions or classes in the project.
Yes, it is.
### 5. Memory Management - meet at least 3 criteria
#### 5.1 The project makes use of references in function declarations.
Yes. Examples include:
- `void HandleInput(Game& game, Controller const& controller)` in State classes
- `void Render(Game& game, Renderer& renderer)` in State classes
- `Snake& GetSnake()` and `SDL_Point& GetFood()` in Game class
- Pass-by-reference used throughout to avoid unnecessary copying

#### 5.2 The project uses destructors appropriately.
Yes. Virtual destructors are implemented:
- Base State class has `virtual ~State() = default;`
- Derived state classes properly inherit virtual destructor
- SDL resources are cleaned up in Renderer destructor
- RAII pattern ensures automatic cleanup

#### 5.3 The project uses scope / Resource Acquisition Is Initialization (RAII) where appropriate.
Yes. RAII is demonstrated through:
- SDL window and renderer resources managed in Renderer class constructor/destructor
- File streams automatically closed when going out of scope in leaderboard functions
- Smart pointers automatically manage state object lifetimes
- Local objects properly cleaned up when leaving scope

#### 5.4 The project follows the Rule of 5.
Yes. Classes that manage resources implement appropriate special member functions:
- State classes use default constructors/destructors appropriately
- Renderer class manages SDL resources with proper cleanup
- Move semantics available for state transitions

#### 5.5 The project uses move semantics to move data instead of copying it, where possible.
Yes. Move semantics used in:
- `ChangeState(std::unique_ptr<State> newState)` uses `std::move()`
- State transitions transfer ownership without copying
- `std::make_unique` creates objects efficiently
- Vector operations use move semantics for leaderboard entries

#### 5.6 The project uses smart pointers instead of raw pointers.
Yes. Smart pointers used throughout:
- `std::unique_ptr<State> currentState` in Game class for state management
- `std::make_unique<PlayingState>()`, `std::make_unique<MenuState>()` for state creation
- Automatic memory management prevents memory leaks
- No raw pointer ownership in the codebase
### 6. Concurrency - meet at least 2 criteria
#### 6.1 The project uses multithreading.
Yes. The project implements two background threads:
- **Food Timer Thread** (`FoodTimerTask()`): Manages food expiration for Advanced/Expert difficulty levels, runs in background to expire food after time limit
- **Leaderboard Thread** (`LoadLeaderboardAsync()`): Asynchronously loads leaderboard data from file to prevent UI blocking during startup
- Threads are properly managed with `std::thread` objects and `join()` operations in `StopBackgroundTasks()`
- Thread lifecycle managed in constructor/destructor and difficulty changes

#### 6.2 A promise and future is used in the project.
Yes. Promise/future pattern implemented for asynchronous leaderboard loading:
- `std::promise<bool> leaderboardPromise` in Game class signals completion of background leaderboard loading
- `std::future<bool> leaderboardFuture` allows main thread to wait for async operation completion
- `WaitForLeaderboardLoad()` method demonstrates `future.get()` usage to retrieve result
- Promise/future pair recreated for each new game session in `StartBackgroundTasks()`
- Proper exception handling prevents "promise already satisfied" errors

#### 6.3 A mutex or lock is used in the project.
Yes. Multiple mutexes ensure thread safety across the application:
- `std::mutex scoreMutex` protects score updates from race conditions between main game thread and potential score modifications
- `std::mutex pauseMutex` synchronizes pause state changes between input handling and game update threads
- `std::lock_guard<std::mutex>` used for automatic RAII-based lock management in critical sections
- Thread-safe score updates implemented in `UpdateScoreThreadSafe()` method
- Mutex protection in `LoadLeaderboardAsync()` prevents concurrent file access

#### 6.4 A condition variable is used in the project.
Yes. Condition variable manages game pause/resume functionality:
- `std::condition_variable pauseCV` blocks game update thread when game is paused
- `Update()` method uses `pauseCV.wait()` with predicate `[this] { return !isPaused || shouldStop; }`
- `ResumeGame()` calls `pauseCV.notify_all()` to wake up waiting game update thread
- Prevents busy waiting and CPU waste during pause state
- Integrates with mutex for proper synchronization of pause state changes

# Overview
* C++/SDL2 Snake with menu, difficulty selection, pause/resume, leaderboard, and timed food (with blinking warning).
* Concurrency is used for background tasks (timed food, leaderboard loading).
# Prerequisites (macOS)
* Install dependencies with Homebrew:
```
brew install cmake sdl2
```
# Build
```
git clone <your-repo-url>
cd CppND-Capstone-Snake-Game
mkdir build && cd build
cmake ..
make -j
```
# Run
```
./SnakeGame
```
# Menu & Navigation
* Main Menu
  * N or 1: New Game
  * L or 2: Leaderboard
  * Q or 3: Quit
* Difficulty Selection
  * 1: Beginner
  * 2: Normal
  * 3: Advanced (timed food + blinking warning)
  * 4: Expert (timed food + blinking warning)
  * ESC: Back to Menu

# In-Game Controls
* Arrow Keys: Move
* SPACE: Pause/Resume
* Q: Quit (works from gameplay and pause)
* Window title shows current score and FPS.
# Gameplay Behavior
* Eating food:
  * Increases score (amount depends on difficulty).
  * Grows the snake (rate depends on difficulty).
  * Slightly increases speed per food.
* Timed Food (Advanced/Expert):
  * Food expires after a time limit.
  * Blinks during the last 3 seconds before disappearing.
  * New food spawns on expiration.
* Game Over:
  * Press Enter to restart, Q to quit.
  * Leaderboard persists at Leaderboard.txt (created on first run). If eligible, you may be prompted in the console for a name.
# Files & Data
* Leaderboard: Leaderboard.txt (auto-created with defaults if missing).
* The game avoids placing food on the snake’s body.
# Troubleshooting
* SDL2 not found:
  * Ensure SDL2 is installed via Homebrew and re-run cmake.
* Thread/pthread link errors:
  * The project links Threads::Threads in CMake; re-run cmake from a clean build folder:
```
cd build
rm -rf *
cmake ..
make -j
```
# Optional (Linux)
```
sudo apt-get install -y cmake libsdl2-dev build-essential
mkdir build && cd build && cmake .. && make -j && ./SnakeGame
```

Tip: Run from the build directory so the app can create/read

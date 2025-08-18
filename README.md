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
Yes, it is.
#### 4.2 Class constructors utilize member initialization lists.
Yes, it is.
#### 4.3 Classes abstract implementation details from their interfaces.
Yes, it is.
#### 4.4 Overloaded functions allow the same function to operate on different parameters.
Yes, it is.
#### 4.5 Classes follow an appropriate inheritance hierarchy with virtual and override functions.
Yes, it is.
#### 4.6 Templates generalize functions or classes in the project.
Yes, it is.
### 5. Memory Management - meet at least 3 criteria
#### 5.1 The project makes use of references in function declarations.
Yes, it is.
#### 5.2 The project uses destructors appropriately.
Yes, it is.
#### 5.3 The project uses scope / Resource Acquisition Is Initialization (RAII) where appropriate.
Yes, it is.
#### 5.4 The project follows the Rule of 5.
Yes, it is.
#### 5.5 The project uses move semantics to move data instead of copying it, where possible.
Yes, it is.
#### 5.6 The project uses smart pointers instead of raw pointers.
Yes, it is.
### 6. Concurrency - meet at least 2 criteria
#### 6.1 The project uses multithreading.
Yes, it is.
#### 6.2 A promise and future is used in the project.
Yes, it is.
#### 6.3 A mutex or lock is used in the project.
Yes, it is.
#### 6.4 A condition variable is used in the project.
Yes, it is.
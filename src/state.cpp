#include "state.h"
#include "game.h"
#include <iostream>

void PlayingState::HandleInput(Game& game, Controller const& controller) {
    bool running = !game.GetPause();
    controller.HandleInput(running, game.GetSnake(), game);
    
    // Only handle state changes
    if (!running && !lastPauseState) {  // Transitioning to pause
        game.PauseGame();
        std::cout << "\nGame Paused - Press SPACE to resume or Q to quit\n";
        lastPauseState = true;
    } else if (running && lastPauseState) {  // Transitioning to resume
        game.ResumeGame();
        std::cout << "\nGame Resumed\n";
        lastPauseState = false;
    }
}

void PlayingState::Update(Game& game) {
    if (!game.GetPause()) {
        Snake& snake = game.GetSnake();
        if (!snake.alive) {
            game.CheckAndUpdateLeaderboard();
            game.ChangeState(std::make_unique<GameOverState>());
            return;
        }
        game.Update();
    }
}

void PlayingState::Render(Game& game, Renderer& renderer) {
    renderer.Render(game.GetSnake(), game.GetFood());
}

void MenuState::Update(Game& game) {
    if (!menuShown) {
        std::cout << "\n=== Snake Game Menu ===\n";
        std::cout << "1. New Game (Press N)\n";
        std::cout << "2. Leaderboard (Press L)\n";
        std::cout << "3. Exit (Press Q)\n";
        menuShown = true;
    }
}

void MenuState::Render(Game& game, Renderer& renderer) {
    SDL_SetWindowTitle(renderer.GetWindow(), "Snake Game - Press N:New Game, L:Leaderboard, Q:Quit");
}

void MenuState::HandleInput(Game& game, Controller const& controller) {
    SDL_Event e;
    while (SDL_PollEvent(&e)) {
        if (e.type == SDL_QUIT) {
            game.GetSnake().alive = false;
        } else if (e.type == SDL_KEYDOWN) {
            switch (e.key.keysym.sym) {
                case SDLK_n:
                    game.ChangeState(std::make_unique<DifficultyState>());
                    break;
                case SDLK_l:
                    game.DisplayLeaderboard();
                    // After showing leaderboard, reprint menu options
                    menuShown = false;  // Reset flag to show menu again
                    break;
                case SDLK_q:
                    game.GetSnake().alive = false;
                    break;
            }
        }
    }
}

void GameOverState::Render(Game& game, Renderer& renderer) {
    renderer.Render(game.GetSnake(), game.GetFood());
    SDL_SetWindowTitle(renderer.GetWindow(), "Game Over! Press Enter to restart or Q to quit");
}

void GameOverState::HandleInput(Game& game, Controller const& controller) {
    SDL_Event e;
    while (SDL_PollEvent(&e)) {
        if (e.type == SDL_QUIT) {
            game.GetSnake().alive = false;
        } else if (e.type == SDL_KEYDOWN) {
            switch (e.key.keysym.sym) {
                case SDLK_RETURN:
                    game.Reset();
                    game.ChangeState(std::make_unique<PlayingState>());
                    break;
                case SDLK_q:
                    game.GetSnake().alive = false;
                    break;
            }
        }
    }
}

void GameOverState::Update(Game& game) {
    // GameOver state doesn't need update logic
}

void DifficultyState::Update(Game& game) {
    if (!menuShown) {
        std::cout << "\n=== Select Difficulty ===\n";
        std::cout << "1. Beginner  - Slower speed, normal scoring\n";
        std::cout << "2. Normal    - Standard speed and scoring\n";
        std::cout << "3. Advanced  - Faster speed, triple scoring, timed food\n";
        std::cout << "4. Expert    - Maximum challenge, highest scoring\n";
        std::cout << "\nPress 1-4 to select difficulty or ESC to return\n";
        menuShown = true;
    }
}

void DifficultyState::Render(Game& game, Renderer& renderer) {
    SDL_SetWindowTitle(renderer.GetWindow(), "Snake Game - Select Difficulty (1-4)");
}

void DifficultyState::HandleInput(Game& game, Controller const& controller) {
    SDL_Event e;
    while (SDL_PollEvent(&e)) {
        if (e.type == SDL_QUIT) {
            game.GetSnake().alive = false;
        } else if (e.type == SDL_KEYDOWN) {
            switch (e.key.keysym.sym) {
                case SDLK_1:
                    game.SetDifficulty(Difficulty::Beginner);
                    game.Reset();
                    game.ChangeState(std::make_unique<PlayingState>());
                    break;
                case SDLK_2:
                    game.SetDifficulty(Difficulty::Normal);
                    game.Reset();
                    game.ChangeState(std::make_unique<PlayingState>());
                    break;
                case SDLK_3:
                    game.SetDifficulty(Difficulty::Advanced);
                    game.Reset();
                    game.ChangeState(std::make_unique<PlayingState>());
                    break;
                case SDLK_4:
                    game.SetDifficulty(Difficulty::Expert);
                    game.Reset();
                    game.ChangeState(std::make_unique<PlayingState>());
                    break;
                case SDLK_ESCAPE:
                    game.ChangeState(std::make_unique<MenuState>());
                    break;
            }
        }
    }
}
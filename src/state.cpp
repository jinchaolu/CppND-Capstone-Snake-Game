#include "state.h"
#include "game.h"
#include <iostream>

void PlayingState::Update(Game& game) {
    if (!game.GetPause()) {
        Snake& snake = game.GetSnake();
        if (!snake.alive) {
            if (game.IsTopScore(game.GetScore())) {
                game.CheckAndUpdateLeaderboard();
            }
            game.ChangeState(std::make_unique<GameOverState>());
            return;
        }
        snake.Update();
    }
}

void PlayingState::Render(Game& game, Renderer& renderer) {
    renderer.Render(game.GetSnake(), game.GetFood());
    std::string title = "Snake Score: " + std::to_string(game.GetScore()) + 
                       (game.GetPause() ? " (Paused)" : "");
    SDL_SetWindowTitle(renderer.GetWindow(), title.c_str());
}

void PlayingState::HandleInput(Game& game, Controller const& controller) {
    controller.HandleInput(game.GetPause(), game.GetSnake());
}

void MenuState::Update(Game& game) {
    // Menu state doesn't need update logic
}

void MenuState::Render(Game& game, Renderer& renderer) {
    SDL_SetWindowTitle(renderer.GetWindow(), 
        "Snake Game - Press N:New Game, L:Leaderboard, Q:Quit");
}

void MenuState::HandleInput(Game& game, Controller const& controller) {
    SDL_Event e;
    while (SDL_PollEvent(&e)) {
        if (e.type == SDL_QUIT) {
            game.GetSnake().alive = false;
        } else if (e.type == SDL_KEYDOWN) {
            switch (e.key.keysym.sym) {
                case SDLK_n:
                    game.Reset();
                    game.ChangeState(std::make_unique<PlayingState>());
                    break;
                case SDLK_l:
                    game.LoadLeaderboard();
                    break;
                case SDLK_q:
                    game.GetSnake().alive = false;
                    break;
            }
        }
    }
}

void GameOverState::Update(Game& game) {
    // Game over state doesn't need update logic
}

void GameOverState::Render(Game& game, Renderer& renderer) {
    renderer.Render(game.GetSnake(), game.GetFood());
    SDL_SetWindowTitle(renderer.GetWindow(), 
        "Game Over! Press Enter to restart, M for menu");
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
                case SDLK_m:
                    game.ChangeState(std::make_unique<MenuState>());
                    break;
            }
        }
    }
}
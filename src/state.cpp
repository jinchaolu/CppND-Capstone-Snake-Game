#include "state.h"
#include "game.h"
#include <iostream>

void PlayingState::HandleInput(Game& game, Controller const& controller) {
    bool running = !game.GetPause(); // Create a local variable that can be passed by reference
    controller.HandleInput(running, game.GetSnake(), game);
    if (!running) {
        game.PauseGame();
    } else {
        game.ResumeGame();
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
    // Menu state doesn't need update logic
}

void MenuState::Render(Game& game, Renderer& renderer) {
    game.ShowMenu();
}

void MenuState::HandleInput(Game& game, Controller const& controller) {
    game.HandleMenuInput();
}

void GameOverState::Update(Game& game) {
    // Game over state doesn't need update logic
}

void GameOverState::Render(Game& game, Renderer& renderer) {
    renderer.Render(game.GetSnake(), game.GetFood());
    SDL_SetWindowTitle(renderer.GetWindow(), "Game Over! Press Enter to restart, M for menu");
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
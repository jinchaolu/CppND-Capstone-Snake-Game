#ifndef STATE_H
#define STATE_H

#include "renderer.h"
class Game;
class Controller;

class State {
 public:
  virtual ~State() = default;
  virtual void Update(Game& game) = 0;
  virtual void Render(Game& game, Renderer& renderer) = 0;
  virtual void HandleInput(Game& game, Controller const& controller) = 0;
};

class PlayingState : public State {
 public:
  PlayingState() : lastPauseState(false) {}
  void Update(Game& game) override;
  void Render(Game& game, Renderer& renderer) override;
  void HandleInput(Game& game, Controller const& controller) override;
 private:
  bool lastPauseState;
};

class MenuState : public State {
 public:
  MenuState() : menuShown(false) {}
  void Update(Game& game) override;
  void Render(Game& game, Renderer& renderer) override;
  void HandleInput(Game& game, Controller const& controller) override;
 private:
  bool menuShown;
};

class GameOverState : public State {
 public:
  void Update(Game& game) override;
  void Render(Game& game, Renderer& renderer) override;
  void HandleInput(Game& game, Controller const& controller) override;
};

#endif
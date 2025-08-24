#include <gtest/gtest.h>
#include "../src/game.h"
#include "../src/state.h"

TEST(StateTest, InitialState) {
    Game game(32, 32);
    EXPECT_NE(nullptr, dynamic_cast<MenuState*>(game.currentState.get()));
}

TEST(StateTest, StateTransition) {
    Game game(32, 32);
    game.ChangeState(std::make_unique<PlayingState>());
    EXPECT_NE(nullptr, dynamic_cast<PlayingState*>(game.currentState.get()));
}
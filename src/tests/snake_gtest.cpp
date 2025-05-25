#include "snake_gtest.h"
#define COPY_FIELD(before, field)        \
  int before[ROWS_MAP][COLS_MAP];        \
  for (int i = 0; i < ROWS_MAP; ++i) {   \
    for (int j = 0; j < COLS_MAP; ++j) { \
      before[i][j] = field[i][j];        \
    }                                    \
  }
#define COMPARE_FIELD(field1, field2, changed) \
  changed = false;                             \
  for (int i = 0; i < ROWS_MAP; ++i) {         \
    for (int j = 0; j < COLS_MAP; ++j) {       \
      if (field1[i][j] != field2[i][j]) {      \
        changed = true;                        \
        i = ROWS_MAP;                          \
        j = COLS_MAP;                          \
      }                                        \
    }                                          \
  }

namespace s21::snake {

class ModelTest : public ::testing::Test {
 protected:
  void SetUp() override {
    model_ = s21::snake::Model::Instance::Get();
    model_->TransitionTo<StartState>();  // std::cout << "SetUp" << std::endl;
  }
  Model* model_ = nullptr;
  void TearDown() override {
    model_->TransitionTo<ExitState>();
    // std::cout << "TearDown" << std::endl;
  }
};

TEST_F(ModelTest, InitGameInfoSetsDefaults) {
  GameInfo_t info = model_->UpdateState();
  EXPECT_EQ(info.score, 0);
  EXPECT_EQ(info.level, 1);
  EXPECT_EQ(info.speed, 1);
  EXPECT_NE(info.field, nullptr);
  EXPECT_FALSE(info.pause);
}

TEST_F(ModelTest, TogglePauseWorks) {
  model_->UserAction(Pause);
  GameInfo_t info = model_->UpdateState();
  EXPECT_TRUE(info.pause);
  model_->UserAction(Pause);
  info = model_->UpdateState();
  EXPECT_FALSE(info.pause);
}

TEST_F(ModelTest, SpawnSnakePlacesSnake) {
  model_->TransitionTo<StartState>();
  GameInfo_t info = model_->UpdateState();
  EXPECT_EQ(info.field[9][3], 0);
  EXPECT_EQ(info.field[9][6], 0);
  model_->UserAction(Start);
  // Check that snake head and tail are set
  EXPECT_NE(info.field[9][3], 0);
  EXPECT_NE(info.field[9][6], 0);
}

TEST_F(ModelTest, SpawnApplePlacesApple) {
  model_->TransitionTo<StartState>();
  model_->UserAction(Start);
  GameInfo_t info = model_->UpdateState();
  bool apple_found = false;
  for (int i = 0; i < ROWS_MAP; ++i) {
    for (int j = 0; j < COLS_MAP; ++j) {
      if (info.field[i][j] & APPLE_MASK) {
        apple_found = true;
        i = ROWS_MAP;
        j = COLS_MAP;  // Break out of both loops
      }
    }
  }
  EXPECT_TRUE(apple_found);
}

TEST_F(ModelTest, PauseDoesNotChangeScoreOrLevel) {
  model_->TransitionTo<StartState>();
  model_->UserAction(Start);
  GameInfo_t before = model_->UpdateState();
  model_->UserAction(Pause);
  GameInfo_t after = model_->UpdateState();
  EXPECT_EQ(before.score, after.score);
  EXPECT_EQ(before.level, after.level);
}

TEST_F(ModelTest, SnakeMovesAfterAction) {
  model_->TransitionTo<StartState>();
  model_->UserAction(Start);
  auto field = model_->UpdateState().field;
  COPY_FIELD(before, field);  // Copy the initial state of the field
  model_->UserAction(Up);     // Try to move up
  model_->UpdateState();      // Need to Update state to apply the move
  // At least one cell should change (snake moves)
  bool changed = false;
  COMPARE_FIELD(before, field, changed);
  EXPECT_TRUE(changed);
}

TEST_F(ModelTest, GameOverOnWallCollision) {
  // Move up until game over (assuming wall at top)
  for (int i = 0; i < ROWS_MAP; ++i) {
    model_->UserAction(Up);
  }
  // After enough moves, the game should be over (score should not increase)
  int score = model_->UpdateState().score;
  model_->UserAction(Up);
  GameInfo_t after = model_->UpdateState();
  EXPECT_EQ(after.score, score);  // Score should not increase after game over
}

TEST_F(ModelTest, RestartGameResetsScore) {
  model_->TransitionTo<StartState>();
  model_->UserAction(Start);
  GameInfo_t info = model_->UpdateState();
  // A little cheat. Add Apple to every free cell
  for (auto i = 1; i < ROWS_MAP - 1; ++i) {
    for (auto j = 1; j < COLS_MAP - 1; ++j) {
      if (info.field[i][j] & HEAD_MASK) {
        info.field[i][j - 1] = APPLE_MASK;
      }
    }
  }
  model_->UserAction(Left);
  info = model_->UpdateState();
  EXPECT_GT(info.score,
            0);  // Score should be greater than 0 after eating an apple
  model_->TransitionTo<GameOverState>();
  model_->UserAction(Start);
  info = model_->UpdateState();
  EXPECT_EQ(info.score, 0);
}

TEST(LibSnakeTest, PauseAndUnpause) {
  Model::Instance::Get()->TransitionTo<StartState>();
  ::userInput(Start, false);
  ::userInput(Pause, false);
  GameInfo_t info = ::updateCurrentState();
  EXPECT_TRUE(info.pause);
  ::userInput(Pause, false);
  info = ::updateCurrentState();
  EXPECT_FALSE(info.pause);
  Model::Instance::Get()->TransitionTo<ExitState>();
}

TEST(LibSnakeTest, TerminateResetsGame) {
  Model::Instance::Get()->TransitionTo<StartState>();
  GameInfo_t info = ::updateCurrentState();
  EXPECT_EQ(info.level, 1);
  EXPECT_NE(info.field, nullptr);  // Field should'nt be nullptr before start
  ::userInput(Start, false);
  ::userInput(Terminate, false);
  info = ::updateCurrentState();
  // After terminate, the game should be reset (score 0, level 1)
  EXPECT_EQ(info.score, 0);
  EXPECT_EQ(info.level, 0);
  EXPECT_EQ(info.field, nullptr);  // Field should be nullptr after exit
}

// libsnake interface tests
TEST(LibSnakeTest, UserInputAndUpdateCurrentState) {
  Model::Instance::Get()->TransitionTo<StartState>();
  ::userInput(Start, false);
  GameInfo_t info = ::updateCurrentState();
  EXPECT_EQ(info.level, 1);
  ::userInput(Terminate, false);
  info = ::updateCurrentState();
  EXPECT_EQ(info.field, nullptr);  // After exit, field should be nullptr
}

// Tests all the signals in the game by sending them in the correct order.
// Starts the game, waits for the update, terminates the game and waits for
// the update again. Verifies that the game state is reset after terminate.
TEST(LibSnakeTest, AllTheSignals) {
  Model::Instance::Get()->TransitionTo<StartState>();
  ::userInput(Start, false);
  GameInfo_t info = ::updateCurrentState();
  ::userInput(Left, false);
  info = ::updateCurrentState();
  ::userInput(Up, false);
  info = ::updateCurrentState();
  ::userInput(Right, false);
  info = ::updateCurrentState();
  ::userInput(Down, false);
  info = ::updateCurrentState();
  for (int i = 0; i < ROWS_MAP; ++i) {
    ::userInput(Action, false);
  }
  info = ::updateCurrentState();
  ::userInput(Start, false);
  info = ::updateCurrentState();
  EXPECT_NE(info.field, nullptr);
  ::userInput(Terminate, false);
  info = ::updateCurrentState();
  EXPECT_EQ(info.field, nullptr);  // After exit, field should be nullptr
}

TEST(LibSnakeTest, TimeR) {
  Model::Instance::Get()->TransitionTo<StartState>();
  ::userInput(Start, false);
  GameInfo_t info = ::updateCurrentState();
  COPY_FIELD(before_field, info.field);
  bool changed = false;
  // Simulate time passing
  std::cout << "Simulating time passage:";
  for (int n = 0; n <= 5; ++n) {
    std::cout << " " << n;
    sleep(1);  // Simulate time passing
    ::updateCurrentState();
    COMPARE_FIELD(before_field, info.field, changed);
    if (changed) {
      std::cout << " (field changed)";
      n = 10;
    }
  }
  std::cout << std::endl;

  EXPECT_TRUE(changed);  // Field should have changed after time simulation
}
}  // namespace s21::snake

#include "snake_gtest.h"

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
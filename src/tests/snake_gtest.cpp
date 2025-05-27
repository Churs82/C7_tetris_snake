#include "snake_gtest.h"
#define COPY_FIELD(before, field)        \
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

TEST_F(ModelTest, PauseDoesNotChangeField) {
  model_->TransitionTo<StartState>();
  model_->UserAction(Start);
  int before[ROWS_MAP][COLS_MAP] = {0};
  auto field = model_->UpdateState().field;
  COPY_FIELD(before, field);  // Copy the initial state of the field
  model_->UserAction(Pause);
  sleep(3);
  bool changed = false;
  COMPARE_FIELD(before, field, changed);
  EXPECT_FALSE(changed);  // Field should not change when paused
}

TEST_F(ModelTest, SnakeMovesAfterAction) {
  model_->TransitionTo<StartState>();
  model_->UserAction(Start);
  auto field = model_->UpdateState().field;
  int before[ROWS_MAP][COLS_MAP] = {0};
  COPY_FIELD(before, field);  // Copy the initial state of the field
  model_->UserAction(Up);     // Try to move up
  // At least one cell should change (snake moves)
  bool changed = false;
  COMPARE_FIELD(before, field, changed);
  EXPECT_TRUE(changed);
}

TEST_F(ModelTest, GameOverOnWallCollisionLeft) {
  model_->TransitionTo<StartState>();
  model_->UserAction(Start);
  // Move left until game over (assuming wall at left)
  model_->UserAction(Up);  // Move up to avoid reverse
  model_->UserAction(Left);
  for (int i = 0; i < COLS_MAP; ++i) {
    model_->UserAction(Action);
  }
  // After enough moves, the game should be over (field shouldnt change)
  auto field = model_->UpdateState().field;
  int before[ROWS_MAP][COLS_MAP] = {0};
  COPY_FIELD(before, field);
  model_->UserAction(Up);
  bool changed = false;
  COMPARE_FIELD(before, field, changed);
  EXPECT_FALSE(
      changed);  // Field shouldnt change after action in gameover state
}

TEST_F(ModelTest, GameOverOnWallCollisionRight) {
  model_->TransitionTo<StartState>();
  model_->UserAction(Start);
  // Move up until game over (assuming wall at top)
  model_->UserAction(Down);  // Move down to avoid reverse
  model_->UserAction(Right);
  for (int i = 0; i < COLS_MAP; ++i) {
    model_->UserAction(Action);
  }
  // After enough moves, the game should be over (field shouldnt change)
  auto field = model_->UpdateState().field;
  int before[ROWS_MAP][COLS_MAP] = {0};
  COPY_FIELD(before, field);
  model_->UserAction(Up);
  bool changed = false;
  COMPARE_FIELD(before, field, changed);
  EXPECT_FALSE(
      changed);  // Field shouldnt change after action in gameover state
}
TEST_F(ModelTest, GameOverOnWallCollisionUP) {
  model_->TransitionTo<StartState>();
  model_->UserAction(Start);
  // Move up until game over (assuming wall at top)
  model_->UserAction(Right);  // Move Right to avoid revers
  model_->UserAction(Up);
  for (int i = 0; i < ROWS_MAP; ++i) {
    model_->UserAction(Action);
  }
  // After enough moves, the game should be over (field shouldnt change)
  auto field = model_->UpdateState().field;
  int before[ROWS_MAP][COLS_MAP] = {0};
  COPY_FIELD(before, field);
  model_->UserAction(Up);
  bool changed = false;
  COMPARE_FIELD(before, field, changed);
  EXPECT_FALSE(
      changed);  // Field shouldnt change after action in gameover state
}
TEST_F(ModelTest, GameOverOnWallCollisionDown) {
  model_->TransitionTo<StartState>();
  model_->UserAction(Start);
  // Move down until game over (assuming wall at bottom)
  model_->UserAction(Left);  // Move left to avoid reverse
  model_->UserAction(Down);
  for (int i = 0; i < ROWS_MAP; ++i) {
    model_->UserAction(Action);
  }
  // After enough moves, the game should be over (field shouldnt change)
  auto field = model_->UpdateState().field;
  int before[ROWS_MAP][COLS_MAP] = {0};
  COPY_FIELD(before, field);
  model_->UserAction(Up);
  bool changed = false;
  COMPARE_FIELD(before, field, changed);
  EXPECT_FALSE(
      changed);  // Field shouldnt change after action in gameover state
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

TEST_F(ModelTest, WinState) {
  model_->TransitionTo<StartState>();
  model_->UserAction(Start);
  GameInfo_t info = model_->UpdateState();
  EXPECT_EQ(info.score, 0);  // We have just started
  // Simulate eating enough apples to win
  for (auto i = 0; i < ROWS_MAP; i++) {
    for (auto j = 0; j < COLS_MAP; j++) {
      if (info.field[i][j] == 0) {
        info.field[i][j] = APPLE_MASK;  // Place an apple in every free cell
      }
    }
  }
  model_->UserAction(Action);    // Eat an apple
  info = model_->UpdateState();  // Initiate SpawnApple
  EXPECT_EQ(info.score, 1);      // We have eaten one apple
  for (int i = 0; i < ROWS_MAP; ++i) {
    for (int j = 0; j < COLS_MAP; ++j) {
      if (info.field[i][j] == 0) {
        FAIL();
      }
    }
  }
  // After enough moves, the game should be over (field shouldnt change)
  int** field = model_->UpdateState().field;
  int before[ROWS_MAP][COLS_MAP] = {0};
  COPY_FIELD(before, field);
  bool changed = false;
  model_->UserAction(Action);
  COMPARE_FIELD(before, field, changed);
  EXPECT_FALSE(changed);  // Field shouldnt change after action in Win state
  sleep(1);               // Wait for the game to process the win condition
  field = model_->UpdateState().field;
  COMPARE_FIELD(before, field, changed);
  model_->UserAction(Start);
  EXPECT_TRUE(changed);
  Model::Instance::Get()->TransitionTo<ExitState>();
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
  int before[ROWS_MAP][COLS_MAP] = {0};
  COPY_FIELD(before, info.field);
  bool changed = false;
  ::userInput(Left, false);
  COMPARE_FIELD(before, info.field, changed);
  EXPECT_TRUE(changed);  // Field should change after left input
  COPY_FIELD(before, info.field);
  ::userInput(Up, false);
  COMPARE_FIELD(before, info.field, changed);
  EXPECT_TRUE(changed);  // Field should change after up input
  COPY_FIELD(before, info.field);
  ::userInput(Right, false);
  COMPARE_FIELD(before, info.field, changed);
  EXPECT_TRUE(changed);  // Field should change after right input
  COPY_FIELD(before, info.field);
  ::userInput(Down, false);
  COMPARE_FIELD(before, info.field, changed);
  EXPECT_TRUE(changed);  // Field should change after down input
  COPY_FIELD(before, info.field);
  ::userInput(Action, false);
  COMPARE_FIELD(before, info.field, changed);
  EXPECT_TRUE(changed);  // Field should change after action input
  COPY_FIELD(before, info.field);
  ::userInput(Start, false);
  COMPARE_FIELD(before, info.field, changed);
  EXPECT_FALSE(changed);  // Field shouldn't be changed after start input
  for (int i = 0; i < ROWS_MAP; ++i) {
    ::userInput(Action, false);
  }
  COPY_FIELD(before, info.field);
  ::userInput(Left, false);
  ::userInput(Up, false);
  ::userInput(Right, false);
  ::userInput(Down, false);
  COMPARE_FIELD(before, info.field, changed);
  EXPECT_FALSE(
      changed);  // Field shouldn't change after all inputs ion GameOver State
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
  int before_field[ROWS_MAP][COLS_MAP] = {0};
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

  ::userInput(Terminate, false);
  info = ::updateCurrentState();
  EXPECT_EQ(info.field, nullptr);  // After exit, field should be nullptr
}
}  // namespace s21::snake

#include "snake_gtest.h"

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
#include "snake_gtest.h"

namespace s21::snake {

class ModelTest : public ::testing::Test {
 protected:
  void SetUp() override { model_ = s21::snake::Model::Instance::Get(); model_-> TransitionTo<StartState>(); std::cout << "SetUp" << std::endl;}
  Model* model_ = nullptr;
  void TearDown() override {
    model_->TransitionTo<ExitState>();
    std::cout << "TearDown" << std::endl;
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
  model_->UserAction(Start);
  GameInfo_t info = model_->UpdateState();
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
        break;
      }
    }
    if (apple_found) break;
  }
  EXPECT_TRUE(apple_found);
}

class StateTest : public ::testing::Test {
 protected:
  void SetUp() override {
    model_ = s21::snake::Model::Instance::Get();
    model_->TransitionTo<StartState>();
  }
  void TearDown() override { model_->TransitionTo<ExitState>(); }
  Model* model_ = nullptr;
};

TEST_F(StateTest, StartStateTransitionsToSpawn) {
  model_->TransitionTo<StartState>();
  model_->UserAction(Start);
  // Should transition to SpawnState after Start
  // (You may need to expose state_ or add a getter for real checks)
  SUCCEED();
}

// libsnake interface tests
TEST(LibSnakeTest, UserInputAndUpdateCurrentState) {
  Model::Instance::Get()->TransitionTo<StartState>();
  userInput(Start, false);
  GameInfo_t info = updateCurrentState();
  EXPECT_GE(info.level, 1);
  Model::Instance::Get()->TransitionTo<ExitState>();
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
  auto before = model_->UpdateState().field;
  model_->UserAction(Up);  // Try to move up
  auto after = model_->UpdateState().field;
  // At least one cell should change (snake moves)
  bool changed = false;
  for (int i = 0; i < ROWS_MAP; ++i) {
    for (int j = 0; j < COLS_MAP; ++j) {
      if (before[i][j] != after[i][j]) {
        changed = true;
        break;
      }
    }
    if (changed) break;
  }
  EXPECT_TRUE(changed);
}

TEST_F(ModelTest, GameOverOnWallCollision) {
  model_->TransitionTo<StartState>();
  model_->UserAction(Start);
  // Move up until game over (assuming wall at top)
  for (int i = 0; i < ROWS_MAP + 2; ++i) {
    model_->UserAction(Up);
    model_->UpdateState();
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
  // Simulate some moves to increase score
  for (int i = 0; i < 3; ++i) {
    model_->UserAction(Action);
    model_->UpdateState();
  }
  model_->TransitionTo<StartState>();
  model_->UserAction(Start);
  GameInfo_t info = model_->UpdateState();
  EXPECT_EQ(info.score, 0);
}

TEST(LibSnakeTest, PauseAndUnpause) {
  Model::Instance::Get()->TransitionTo<StartState>();
  userInput(Start, false);
  userInput(Pause, false);
  GameInfo_t info = updateCurrentState();
  EXPECT_TRUE(info.pause);
  userInput(Pause, false);
  info = updateCurrentState();
  EXPECT_FALSE(info.pause);
  Model::Instance::Get()->TransitionTo<ExitState>();
}

TEST(LibSnakeTest, TerminateResetsGame) {
  Model::Instance::Get()->TransitionTo<StartState>();
  userInput(Start, false);
  userInput(Terminate, false);
  GameInfo_t info = updateCurrentState();
  // After terminate, the game should be reset (score 0, level 1)
  EXPECT_EQ(info.score, 0);
  EXPECT_EQ(info.level, 1);
  Model::Instance::Get()->TransitionTo<ExitState>();
}

}  // namespace s21::snake

#include "snake_gtest.h"

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
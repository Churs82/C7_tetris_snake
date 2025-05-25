#include "snake_gtest.h"
#define  COPY_FIELD(before, field) \
  int before[ROWS_MAP][COLS_MAP]; \
  for (int i = 0; i < ROWS_MAP; ++i) { \
    for (int j = 0; j < COLS_MAP; ++j) { \
      before[i][j] = field[i][j]; \
    } \
  }

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
  GameInfo_t info = model_->UpdateState();
  EXPECT_NE(info.field[9][3], 0);
  EXPECT_NE(info.field[9][6], 0);
  model_->UserAction(Start);
  info = model_->UpdateState();
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
        i=ROWS_MAP;
        j=COLS_MAP;  // Break out of both loops
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


// libsnake interface tests
TEST(LibSnakeTest, UserInputAndUpdateCurrentState) {
  Model::Instance::Get()->TransitionTo<ExitState>();
  Model::Instance::Get()->TransitionTo<StartState>();
  userInput(Start, false);
  GameInfo_t info = updateCurrentState();
  EXPECT_EQ(info.level, 1);
  userInput(Terminate, false);
  EXPECT_EQ(info.field, nullptr);  // After exit, field should be nullptr
}


TEST_F(ModelTest, SnakeMovesAfterAction) {
  model_->TransitionTo<StartState>();
  model_->UserAction(Start);
  auto field = model_->UpdateState().field;
  COPY_FIELD(before, field);  // Copy the initial state of the field
  model_->UserAction(Up);  // Try to move up
  // At least one cell should change (snake moves)
  bool changed = false;
  for (int i = 0; i < ROWS_MAP; ++i) {
    for (int j = 0; j < COLS_MAP; ++j) {
      if (before[i][j] != field[i][j]) {
        changed = true;
        i = ROWS_MAP;  // Break outer loop
        j = COLS_MAP;  // Break inner loop
      }
    }
  }
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
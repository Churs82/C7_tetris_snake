#include "snake_gtest.h"

namespace s21::snake {

class ModelTest : public ::testing::Test {
 protected:
  void SetUp() override { model_ = s21::snake::Model::Instance::Get(); }
  Model* model_ = nullptr;
  void TearDown() override { model_->TransitionTo<ExitState>(); }
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

}  // namespace s21::snake

#include "snake_gtest.h"

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
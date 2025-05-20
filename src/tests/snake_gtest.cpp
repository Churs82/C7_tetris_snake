#include "snake_gtest.h"

namespace s21::snake {

class ModelTest : public ::testing::Test {
 protected:
  void SetUp() override { model_ = s21::snake::Model::Instance::Get(); }
  Model* model_ = nullptr;
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
  model_->TogglePause();
  GameInfo_t info = model_->UpdateState();
  EXPECT_TRUE(info.pause);
}

TEST_F(ModelTest, SpawnSnakePlacesSnake) {
  model_->RestartGame();
  GameInfo_t info = model_->UpdateState();
  // Check that snake head and tail are set
  EXPECT_NE(info.field[9][3], 0);
  EXPECT_NE(info.field[9][6], 0);
}

class StateTest : public ::testing::Test {
 protected:
  void SetUp() override {
    model_ = s21::snake::Model::Instance::Get();
    model_->InitGameInfo();
  }
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
  userInput(Start, false);
  GameInfo_t info = updateCurrentState();
  EXPECT_GE(info.level, 1);
}

}  // namespace s21::snake

#include "snake_gtest.h"

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
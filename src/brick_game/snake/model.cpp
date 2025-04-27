#include "model.h"

namespace s21::snake {

model::model() { TransitionTo<Start_state>(); }

void model::TogglePause() { game_info_->pause = !game_info_->pause; }
void model::DeleteGI() {
  if (game_info_.get() != nullptr) {
    game_info_->speed = 0;
    game_info_->level = 0;
    if (game_info_->field != nullptr) {
      for (auto i = 0; i < ROWS_MAP; i++) {
        delete[] game_info_->field[i];
      }
      delete[] game_info_->field;
      game_info_->field = nullptr;
    }
    if (game_info_->next != nullptr) {
      for (auto i = 0; i < FIGURE_H; i++) {
        delete[] game_info_->next[i];
      }
      delete[] game_info_->next;
      game_info_->next = nullptr;
    }
  }
}
void model::InitGI() {
  // game_info_ = GI_unique_ptr(new GameInfo_t, [this] { this->DeleteGI(); });
  game_info_ = GI_unique_ptr(new GameInfo_t{0});
  game_info_->pause = false;
  game_info_->score = 0;
  game_info_->level = 1;
  game_info_->speed = 1;
  game_info_->field = new int* [ROWS_MAP] { 0 };
  for (int i = 0; i < ROWS_MAP; i++) {
    game_info_->field[i] = new int[COLS_MAP]{0};
  }
  game_info_->next = new int* [FIGURE_H] { 0 };
  for (int i = 0; i < FIGURE_H; i++) {
    game_info_->next[i] = new int[FIGURE_W]{0};
  }
  std::srand(std::time(nullptr));
}
/* Common State functions */
void State::Terminate() { fsm_->TransitionTo<Exit_state>(); }
void State::Pause() { fsm_->TogglePause(); };

/* Gamelogic functions */
void model::SpawnSnake() {
  game_info_->field[9][3] =
      SNAKE_MASK + HEAD_MASK + (DIRECTION_MASK & DIRECTION_LEFT);
  game_info_->field[9][4] = SNAKE_MASK + (DIRECTION_MASK & DIRECTION_LEFT);
  game_info_->field[9][5] = SNAKE_MASK + (DIRECTION_MASK & DIRECTION_LEFT);
  game_info_->field[9][6] = SNAKE_MASK + (DIRECTION_MASK & DIRECTION_LEFT);
}

void model::SpawnApple() {
  int x = std::rand() % (COLS_MAP - 1);
  int y = std::rand() % (ROWS_MAP - 1);
  if (game_info_->field[y][x] != 0)
    SpawnApple();
  else
    game_info_->field[y][x] = APPLE_MASK;
}

};  // namespace s21::snake

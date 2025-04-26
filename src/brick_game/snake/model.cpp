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
  game_info_ = GI_unique_ptr(new GameInfo_t);
  game_info_->pause = false;
  game_info_->score = 0;
  game_info_->level = 1;
  game_info_->speed = 1;
  game_info_->field = new int*[ROWS_MAP];
  for (int i = 0; i < ROWS_MAP; i++) {
    game_info_->field[i] = new int[COLS_MAP];
    for (int j = 0; j < COLS_MAP; j++) {
      game_info_->field[i][j] = 0;
    }
  }
  game_info_->next = new int*[FIGURE_H];
  for (int i = 0; i < FIGURE_H; i++) {
    game_info_->next[i] = new int[FIGURE_W];
    for (int j = 0; j < FIGURE_W; j++) {
      game_info_->next[i][j] = 0;
    }
  }
}
/* Common State functions */
void State::Terminate() { fsm_->TransitionTo<Exit_state>(); }
void State::Pause() { fsm_->TogglePause(); };

};  // namespace s21::snake

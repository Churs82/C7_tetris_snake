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
  s_head = {9, 3};
  s_tail = {9, 6};
  game_info_->field[9][3] = SNAKE_MASK | DIRECTION_UP | HEAD_MASK;
  game_info_->field[9][4] = SNAKE_MASK | DIRECTION_LEFT;
  game_info_->field[9][5] = SNAKE_MASK | DIRECTION_LEFT;
  game_info_->field[9][6] = SNAKE_MASK | DIRECTION_LEFT | TAIL_MASK;
}

void model::SpawnApple() {
  int x = std::rand() % COLS_MAP;
  int y = std::rand() % ROWS_MAP;
  if (game_info_->field[y][x] != 0)
    SpawnApple();
  else
    game_info_->field[y][x] = APPLE_MASK;
}

void model::StartTimer() { timer_ = steady_clock::now(); }

void model::CheckTimer() {
  if (duration_cast<milliseconds>(steady_clock::now() - timer_).count() >=
      2000 / game_info_->speed)
    TransitionTo<Moving_state>();
}

void model::MoveSnake() {
  int direction = game_info_->field[s_head[0]][s_head[1]] & DIRECTION_MASK;
  game_info_->field[s_head[0]][s_head[1]] &= ~HEAD_MASK;
  switch (direction) {
    case DIRECTION_UP:
      s_head[0]++;
      break;
    case DIRECTION_DOWN:
      s_head[0]--;
      break;
    case DIRECTION_LEFT:
      s_head[1]--;
      break;
    case DIRECTION_RIGHT:
      s_head[1]++;
      break;
  }
  /* Проверка на выход за края поля */
  if (s_head[1] < 0 || s_head[1] >= COLS_MAP || s_head[0] < 0 ||
      s_head[0] >= ROWS_MAP) {
    TransitionTo<Exit_state>();
  } else {
    if (game_info_->field[s_head[0]][s_head[1]] & APPLE_MASK) {
      game_info_->score++; /*TODO: snake::addScore() */
      game_info_->field[s_head[0]][s_head[1]] =
          SNAKE_MASK | HEAD_MASK | direction;
      TransitionTo<Spawn_state>();
    } else {
      switch (game_info_->field[s_tail[0]][s_tail[1]] & DIRECTION_MASK) {
        case DIRECTION_UP:
          game_info_->field[s_tail[0]++][s_tail[1]] = 0;
          break;
        case DIRECTION_DOWN:
          game_info_->field[s_tail[0]--][s_tail[1]] = 0;
          break;
        case DIRECTION_LEFT:
          game_info_->field[s_tail[0]][s_tail[1]--] = 0;
          break;
        case DIRECTION_RIGHT:
          game_info_->field[s_tail[0]][s_tail[1]++] = 0;
          break;
      }
      game_info_->field[s_tail[0]][s_tail[1]] |= TAIL_MASK;
      if (game_info_->field[s_head[0]][s_head[1]] & SNAKE_MASK) {
        TransitionTo<Exit_state>();
      } else {
        game_info_->field[s_head[0]][s_head[1]] =
            SNAKE_MASK | HEAD_MASK | direction;
      }
    }
  }
}

void model::RotateLeft() {
  if (s_head[1] - 1 > 0 && s_head[1] - 1 < COLS_MAP &&
      (game_info_->field[s_head[0]][s_head[1] - 1] == 0 ||
       game_info_->field[s_head[0]][s_head[1] - 1] & TAIL_MASK ||
       game_info_->field[s_head[0]][s_head[1] - 1] & APPLE_MASK)) {
    game_info_->field[s_head[0]][s_head[1]] &= ~DIRECTION_MASK;
    game_info_->field[s_head[0]][s_head[1]] |= DIRECTION_LEFT;
  }
}

void model::RotateRight() {
  if (s_head[1] + 1 > 0 && s_head[1] + 1 < COLS_MAP &&
      (game_info_->field[s_head[0]][s_head[1] + 1] == 0 ||
       game_info_->field[s_head[0]][s_head[1] + 1] & TAIL_MASK ||
       game_info_->field[s_head[0]][s_head[1] + 1] & APPLE_MASK)) {
    game_info_->field[s_head[0]][s_head[1]] &= ~DIRECTION_MASK;
    game_info_->field[s_head[0]][s_head[1]] |= DIRECTION_RIGHT;
  }
}

void model::RotateUp() {
  if (s_head[0] + 1 > 0 && s_head[0] + 1 < ROWS_MAP &&
      (game_info_->field[s_head[0] + 1][s_head[1]] == 0 ||
       game_info_->field[s_head[0] + 1][s_head[1]] & TAIL_MASK ||
       game_info_->field[s_head[0] + 1][s_head[1]] & APPLE_MASK)) {
    game_info_->field[s_head[0]][s_head[1]] &= ~DIRECTION_MASK;
    game_info_->field[s_head[0]][s_head[1]] |= DIRECTION_UP;
  }
}

void model::RotateDown() {
  if (s_head[0] - 1 > 0 && s_head[0] - 1 < ROWS_MAP &&
      (game_info_->field[s_head[0] - 1][s_head[1]] == 0 ||
       game_info_->field[s_head[0] - 1][s_head[1]] & TAIL_MASK ||
       game_info_->field[s_head[0] - 1][s_head[1]] & APPLE_MASK)) {
    game_info_->field[s_head[0]][s_head[1]] &= ~DIRECTION_MASK;
    game_info_->field[s_head[0]][s_head[1]] |= DIRECTION_DOWN;
  }
}

};  // namespace s21::snake

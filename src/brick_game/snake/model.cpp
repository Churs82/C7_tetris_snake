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
  game_info_->field[9][3] = SNAKE_MASK | DIRECTION_LEFT | HEAD_MASK;
  game_info_->field[9][4] = SNAKE_MASK | DIRECTION_LEFT;
  game_info_->field[9][5] = SNAKE_MASK | DIRECTION_LEFT;
  game_info_->field[9][6] = SNAKE_MASK | DIRECTION_LEFT | TAIL_MASK;
}

void model::SpawnApple() {
  for (int i = 0; i < ROWS_MAP; i++) {
    for (int j = 0; j < COLS_MAP; j++) {
      if (game_info_->field[i][j] == APPLE_MASK) game_info_->field[i][j] = 0;
    }
  }
  int x = std::rand() % (COLS_MAP - 1);
  int y = std::rand() % (ROWS_MAP - 1);
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
  std::array<int, 2> s_head{0};
  std::array<int, 2> s_tail{0};
  for (int i = ROWS_MAP - 1; i > 0; i--) {
    for (int j = COLS_MAP - 1; j > 0; j--) {
      /* ищем голову змеи */
      if (game_info_->field[i][j] & HEAD_MASK &&
          game_info_->field[i][j] & SNAKE_MASK) {
        s_head = {i, j};
      }
      /* ищем хвост змеи */
      if (game_info_->field[i][j] & TAIL_MASK &&
          game_info_->field[i][j] & SNAKE_MASK) {
        s_tail = {i, j};
      }
    }
  }
  int direction = game_info_->field[s_head[0]][s_head[1]] & DIRECTION_MASK;
  int next_x = s_head[1];
  int next_y = s_head[0];
  game_info_->field[s_head[0]][s_head[1]] &= ~HEAD_MASK;
  switch (direction) {
    case DIRECTION_UP:
      next_y--;
      break;
    case DIRECTION_DOWN:
      next_y++;
      break;
    case DIRECTION_LEFT:
      next_x--;
      break;
    case DIRECTION_RIGHT:
      next_x++;
      break;
  }

  if (next_x < 0 || next_x >= COLS_MAP || next_y < 0 || next_y >= ROWS_MAP) {
    TransitionTo<Exit_state>();
  } else {
    if (game_info_->field[next_y][next_x] & APPLE_MASK) {
      game_info_->score++;
      game_info_->field[next_y][next_x] = SNAKE_MASK | HEAD_MASK | direction;
      TransitionTo<Spawn_state>();
    } else {
      switch (game_info_->field[s_tail[0]][s_tail[1]] & DIRECTION_MASK) {
        case DIRECTION_UP:
          game_info_->field[s_tail[0] - 1][s_tail[1]] |= TAIL_MASK;
          break;
        case DIRECTION_DOWN:
          game_info_->field[s_tail[0] + 1][s_tail[1]] |= TAIL_MASK;
          break;
        case DIRECTION_LEFT:
          game_info_->field[s_tail[0]][s_tail[1] - 1] |= TAIL_MASK;
          break;
        case DIRECTION_RIGHT:
          game_info_->field[s_tail[0]][s_tail[1] + 1] |= TAIL_MASK;
          break;
      }
      game_info_->field[s_tail[0]][s_tail[1]] = 0;
      if (game_info_->field[next_y][next_x] & SNAKE_MASK) {
        TransitionTo<Exit_state>();
      } else {
        game_info_->field[next_y][next_x] = SNAKE_MASK | HEAD_MASK | direction;
      }
    }
  }
}

};  // namespace s21::snake

#include "model.h"

#include "states.h"

namespace s21::snake {

Model::Model() noexcept { TransitionTo<StartState>(); }
Model::~Model() noexcept { delete state_; }

void Model::UserAction(const UserAction_t action) noexcept {
  if (state_->action_map.size() > action) {
    if (action != Pause) game_info_->pause = 0;
    state_->action_map[action]();
  }
}

GameInfo_t Model::UpdateState() noexcept {
  if (!game_info_->pause) state_->Update();
  return *game_info_;
}

void Model::TogglePause() noexcept { game_info_->pause = !game_info_->pause; }

void Model::DeleteGameInfo() noexcept {
  if (game_info_.get() != nullptr) {
    game_info_->score = 0;
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
void Model::InitGameInfo() noexcept {
  if (game_info_.get() != nullptr) {
    DeleteGameInfo();
  } else {
    game_info_ = std::unique_ptr<GameInfo_t>(new GameInfo_t{0});
  };

  game_info_.get()->pause = false;
  game_info_.get()->score = 0;
  game_info_.get()->level = 1;
  game_info_.get()->speed = 1;
  game_info_.get()->field = new int* [ROWS_MAP] { 0 };
  for (int i = 0; i < ROWS_MAP; i++) {
    game_info_.get()->field[i] = new int[COLS_MAP]{0};
  }
  game_info_.get()->next = new int* [FIGURE_H] { 0 };
  for (int i = 0; i < FIGURE_H; i++) {
    game_info_.get()->next[i] = new int[FIGURE_W]{0};
  }
  std::srand(std::time(nullptr));
}

/* Gamelogic functions */
void Model::SpawnSnake() noexcept {
  snake_head_ = {9, 3};
  snake_tail_ = {9, 6};
  game_info_->field[9][3] = SNAKE_MASK | DIRECTION_UP | HEAD_MASK;
  game_info_->field[9][4] = SNAKE_MASK | DIRECTION_LEFT;
  game_info_->field[9][5] = SNAKE_MASK | DIRECTION_LEFT;
  game_info_->field[9][6] = SNAKE_MASK | DIRECTION_LEFT | TAIL_MASK;
}

void Model::SpawnApple() noexcept {
  int x = std::rand() % COLS_MAP;
  int y = std::rand() % ROWS_MAP;
  if (game_info_->field[y][x] != 0)
    SpawnApple();
  else
    game_info_->field[y][x] = APPLE_MASK;
}

void Model::StartTimer() noexcept { timer_ = steady_clock::now(); }

void Model::CheckTimer() noexcept {
  if (duration_cast<milliseconds>(steady_clock::now() - timer_).count() >
      250 + 2000 / game_info_->speed) {
    TransitionTo<MovingState>();
  }
}

void Model::MoveSnake() noexcept {
  int direction =
      game_info_->field[snake_head_[0]][snake_head_[1]] & DIRECTION_MASK;
  game_info_->field[snake_head_[0]][snake_head_[1]] &= ~HEAD_MASK;
  switch (direction) {
    case DIRECTION_UP:
      ++snake_head_[0];
      break;
    case DIRECTION_DOWN:
      --snake_head_[0];
      break;
    case DIRECTION_LEFT:
      --snake_head_[1];
      break;
    case DIRECTION_RIGHT:
      ++snake_head_[1];
      break;
  }
  /* Проверка на выход за края поля или пересечение с хвостом*/
  if (!CheckBounds(snake_head_)) {
    TransitionTo<GameOverState>();
  } else {
    if (game_info_->field[snake_head_[0]][snake_head_[1]] & APPLE_MASK) {
      AddScore();
      game_info_->field[snake_head_[0]][snake_head_[1]] =
          SNAKE_MASK | HEAD_MASK | direction;
      TransitionTo<SpawnState>();
    } else {
      MoveTail();
      game_info_->field[snake_head_[0]][snake_head_[1]] =
          SNAKE_MASK | HEAD_MASK | direction;
      TransitionTo<RotationState>();
    }
  }
}

void Model::MoveTail() noexcept {
  switch (game_info_->field[snake_tail_[0]][snake_tail_[1]] & DIRECTION_MASK) {
    case DIRECTION_UP:
      game_info_->field[snake_tail_[0]++][snake_tail_[1]] = 0;
      break;
    case DIRECTION_DOWN:
      game_info_->field[snake_tail_[0]--][snake_tail_[1]] = 0;
      break;
    case DIRECTION_LEFT:
      game_info_->field[snake_tail_[0]][snake_tail_[1]--] = 0;
      break;
    case DIRECTION_RIGHT:
      game_info_->field[snake_tail_[0]][snake_tail_[1]++] = 0;
      break;
  }
  game_info_->field[snake_tail_[0]][snake_tail_[1]] |= TAIL_MASK;
}
void Model::RotateLeft() noexcept {
  if (CheckBounds({snake_head_[0], snake_head_[1] - 1})) {
    SetDirection(DIRECTION_LEFT);
    TransitionTo<MovingState>();
  }
}

void Model::RotateRight() noexcept {
  if (CheckBounds({snake_head_[0], snake_head_[1] + 1})) {
    SetDirection(DIRECTION_RIGHT);
    TransitionTo<MovingState>();
  }
}

void Model::RotateUp() noexcept {
  if (CheckBounds({snake_head_[0] + 1, snake_head_[1]})) {
    SetDirection(DIRECTION_UP);
    TransitionTo<MovingState>();
  }
}

void Model::RotateDown() noexcept {
  if (CheckBounds({snake_head_[0] - 1, snake_head_[1]})) {
    SetDirection(DIRECTION_DOWN);
    TransitionTo<MovingState>();
  }
}

bool Model::CheckBounds(const std::array<int, 2>& next) const noexcept {
  return (next[0] >= 0 && next[0] < ROWS_MAP && next[1] >= 0 &&
          next[1] < COLS_MAP &&
          (game_info_->field[next[0]][next[1]] == 0 ||
           game_info_->field[next[0]][next[1]] & TAIL_MASK ||
           game_info_->field[next[0]][next[1]] & APPLE_MASK));
}

void Model::SetDirection(const int direction) noexcept {
  game_info_->field[snake_head_[0]][snake_head_[1]] &= ~DIRECTION_MASK;
  game_info_->field[snake_head_[0]][snake_head_[1]] |= direction;
}

void Model::AddScore(const int score_num) noexcept {
  game_info_->score += score_num;
  game_info_->speed = game_info_->level = game_info_->score / 5 + 1;
  if (game_info_->score >= ROWS_MAP * COLS_MAP - 4) TransitionTo<WinState>();
  if (game_info_->high_score < game_info_->score)
    game_info_->high_score = game_info_->score;
}

void Model::SplashField() noexcept {
  if (duration_cast<milliseconds>(steady_clock::now() - timer_).count() > 500) {
    StartTimer();
    for (auto i = 0; i < ROWS_MAP; i++) {
      for (auto j = 0; j < COLS_MAP; j++) {
        game_info_->field[i][j] = !game_info_->field[i][j];
      }
    }
  }
}

void Model::SaveScore() noexcept {
  std::ofstream file(HS_FILE);
  if (file.is_open()) {
    file << game_info_->high_score;
    file.close();
  }
}

void Model::LoadScore() noexcept {
  std::ifstream file(HS_FILE);
  if (file.is_open()) {
    file >> game_info_->high_score;
    file.close();
  }
}

void Model::RestartGame() noexcept {
  TransitionTo<ExitState>();
  TransitionTo<StartState>();
  state_->Start();
}
};  // namespace s21::snake

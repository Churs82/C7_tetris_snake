#define IM_STATE

#include "states.h"

namespace s21::snake {

/* Common State functions */
void State::Terminate() noexcept {
  this->model_->TransitionTo<ExitState>();
  return;
}
void State::Pause() noexcept {
  this->model_->TogglePause();
  return;
}

// StartState definitions
STATE_CLASS_CONSTRUCT(Start)

this->model_->InitGameInfo();
this->model_->LoadScore();
}
void StartState::Start() noexcept {
  this->model_->SpawnSnake();
  this->model_->TransitionTo<SpawnState>();
}

// SpawnState definitions
STATE_CLASS_CONSTRUCT(Spawn)
this->model_->SpawnApple();
}
void SpawnState::Update() noexcept {
  this->model_->TransitionTo<RotationState>();
}

// RotationState definitions
STATE_CLASS_CONSTRUCT(Rotation)
this->model_->StartTimer();
}
void RotationState::Update() noexcept { this->model_->CheckTimer(); }
void RotationState::Left() noexcept { this->model_->RotateLeft(); }
void RotationState::Right() noexcept { this->model_->RotateRight(); }
void RotationState::Down() noexcept { this->model_->RotateDown(); }
void RotationState::Up() noexcept { this->model_->RotateUp(); }
void RotationState::Action() noexcept {
  this->model_->TransitionTo<MovingState>();
}

// MovingState definitions
STATE_CLASS_CONSTRUCT(Moving)
this->model_->StartTimer();
}

void MovingState::Update() noexcept { this->model_->MoveSnake(); }
MovingState::~MovingState() noexcept { this->model_->StartTimer(); }

// ExitState definitions
STATE_CLASS_CONSTRUCT(Exit)
this->model_->DeleteGameInfo();
}

// GameOverState definitions
STATE_CLASS_CONSTRUCT(GameOver)
this->model_->SaveScore();
}
void GameOverState::Update() noexcept { this->model_->SplashField(); }
void GameOverState::Start() noexcept { this->model_->RestartGame(); }

// WinState definitions
STATE_CLASS_CONSTRUCT(Win)
this->model_->SaveScore();
}
void WinState::Update() noexcept { this->model_->SplashField(); }
void WinState::Start() noexcept { model_->RestartGame(); }

}  // namespace s21::snake

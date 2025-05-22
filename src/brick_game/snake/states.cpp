#include "states.h"

namespace s21::snake {

// State class definitions
State::State() {}
State::State(Model* model) noexcept : model_(model) {}

/* Common State functions */
void State::Terminate() noexcept {
  model_->TransitionTo<ExitState>();
  return;
}
void State::Pause() noexcept {
  model_->TogglePause();
  return;
}

// StartState definitions
StartState::StartState(Model* model) noexcept {
  model_ = model;
  model_->InitGameInfo();
  model_->LoadScore();
}
void StartState::Start() noexcept {
  model_->SpawnSnake();
  model_->TransitionTo<SpawnState>();
}

// SpawnState definitions
SpawnState::SpawnState(Model* model) noexcept {
  model_ = model;
  model_->SpawnApple();
}
void SpawnState::Update() noexcept { model_->TransitionTo<RotationState>(); }

// RotationState definitions
RotationState::RotationState(Model* model) noexcept {
  model_ = model;
  model_->StartTimer();
}
void RotationState::Update() noexcept { model_->CheckTimer(); }
void RotationState::Left() noexcept { model_->RotateLeft(); }
void RotationState::Right() noexcept { model_->RotateRight(); }
void RotationState::Down() noexcept { model_->RotateDown(); }
void RotationState::Up() noexcept { model_->RotateUp(); }
void RotationState::Action() noexcept { model_->TransitionTo<MovingState>(); }

// MovingState definitions
MovingState::MovingState(Model* model) noexcept { model_ = model; }
void MovingState::Update() noexcept { model_->MoveSnake(); }
MovingState::~MovingState() noexcept { model_->StartTimer(); }

// ExitState definitions
ExitState::ExitState(Model* model) noexcept {
  model_ = model;
  model_->DeleteGameInfo();
}

// GameOverState definitions
GameOverState::GameOverState(Model* model) noexcept {
  model_ = model;
  model_->SaveScore();
}
void GameOverState::Update() noexcept { model_->SplashField(); }
void GameOverState::Start() noexcept { model_->RestartGame(); }

// WinState definitions
WinState::WinState(Model* model) noexcept {
  model_ = model;
  model_->SaveScore();
}
void WinState::Update() noexcept { model_->SplashField(); }
void WinState::Start() noexcept { model_->RestartGame(); }

}  // namespace s21::snake

#include "states.h"

namespace s21::snake {

// State class definitions
State::State() {}
State::State(Model* model) : model_(model) {}

/* Common State functions */
void State::Terminate() {
  model_->TransitionTo<ExitState>();
  return;
}
void State::Pause() {
  model_->TogglePause();
  return;
}

// StartState definitions
StartState::StartState(Model* model) {
  model_ = model;
  model_->InitGameInfo();
  model_->LoadScore();
}
void StartState::Start() {
  model_->SpawnSnake();
  model_->TransitionTo<SpawnState>();
}

// SpawnState definitions
SpawnState::SpawnState(Model* model) {
  model_ = model;
  model_->SpawnApple();
}
void SpawnState::Update() { model_->TransitionTo<RotationState>(); }

// RotationState definitions
RotationState::RotationState(Model* model) {
  model_ = model;
  model_->StartTimer();
}
void RotationState::Update() { model_->CheckTimer(); }
void RotationState::Left() { model_->RotateLeft(); }
void RotationState::Right() { model_->RotateRight(); }
void RotationState::Down() { model_->RotateDown(); }
void RotationState::Up() { model_->RotateUp(); }
void RotationState::Action() { model_->TransitionTo<MovingState>(); }

// MovingState definitions
MovingState::MovingState(Model* model) { model_ = model; }
void MovingState::Update() { model_->MoveSnake(); }
MovingState::~MovingState() { model_->StartTimer(); }

// ExitState definitions
ExitState::ExitState(Model* model) {
  model_ = model;
  model_->DeleteGameInfo();
}

// GameOverState definitions
GameOverState::GameOverState(Model* model) {
  model_ = model;
  model_->SaveScore();
}
void GameOverState::Update() { model_->SplashField(); }
void GameOverState::Start() { model_->RestartGame(); }

// WinState definitions
WinState::WinState(Model* model) {
  model_ = model;
  model_->SaveScore();
}
void WinState::Update() { model_->SplashField(); }
void WinState::Start() { model_->RestartGame(); }

}  // namespace s21::snake

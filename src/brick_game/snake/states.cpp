#include "states.h"

namespace s21::snake {

// State class definitions
State::State() {}
State::State(model* fsm) : fsm_(fsm) {}
void State::Update() {}
void State::Start() {}
/* Common State functions */
void State::Terminate() { fsm_->TransitionTo<Exit_state>(); }
void State::Pause() { fsm_->TogglePause(); };
void State::Left() {}
void State::Right() {}
void State::Down() {}
void State::Up() {}
void State::Action() {}
State::~State() {}

// Start_state definitions
Start_state::Start_state(model* fsm) {
  fsm_ = fsm;
  fsm_->InitGI();
  fsm_->LoadScore();
}
void Start_state::Update() { fsm_->TransitionTo<Spawn_state>(); }
Start_state::~Start_state() { fsm_->SpawnSnake(); }

// Spawn_state definitions
Spawn_state::Spawn_state(model* fsm) {
  fsm_ = fsm;
  fsm_->SpawnApple();
}
void Spawn_state::Update() { fsm_->TransitionTo<Rotation_state>(); }

// Rotation_state definitions
Rotation_state::Rotation_state(model* fsm) {
  fsm_ = fsm;
  fsm_->StartTimer();
}
void Rotation_state::Update() { fsm_->CheckTimer(); }
void Rotation_state::Left() { fsm_->RotateLeft(); }
void Rotation_state::Right() { fsm_->RotateRight(); }
void Rotation_state::Down() { fsm_->RotateDown(); }
void Rotation_state::Up() { fsm_->RotateUp(); }
void Rotation_state::Action() { fsm_->TransitionTo<Moving_state>(); }

// Moving_state definitions
Moving_state::Moving_state(model* fsm) {
  fsm_ = fsm;
  fsm_->MoveSnake();
}
void Moving_state::Update() { fsm_->TransitionTo<Rotation_state>(); }

// Exit_state definitions
Exit_state::Exit_state(model* fsm) {
  fsm_ = fsm;
  fsm_->DeleteGI();
}

// GameOver_state definitions
GameOver_state::GameOver_state(model* fsm) {
  fsm_ = fsm;
  fsm_->SaveScore();
}
void GameOver_state::Update() { fsm_->SplashField(); }
void GameOver_state::Start() {
  fsm_->DeleteGI();
  fsm_->TransitionTo<Start_state>();
}

// Win_state definitions
Win_state::Win_state(model* fsm) {
  fsm_ = fsm;
  fsm_->SaveScore();
}
void Win_state::Update() { fsm_->SplashField(); }
void Win_state::Start() {
  fsm_->DeleteGI();
  fsm_->TransitionTo<Start_state>();
}

}  // namespace s21::snake

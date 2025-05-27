#define IM_STATE

#include "states.h"

namespace s21::snake {

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
STATE_CLASS_CONSTRUCT(Start)

model_->InitGameInfo();
model_->LoadScore();
}
void StartState::Start() noexcept {
  model_->SpawnSnake();
  model_->TransitionTo<SpawnState>();
}

// SpawnState definitions
STATE_CLASS_CONSTRUCT(Spawn)
}
void SpawnState::Update() noexcept { model_->SpawnApple(); }

// RotationState definitions
STATE_CLASS_CONSTRUCT(Rotation)
model_->StartTimer();
}
void RotationState::Update() noexcept { model_->CheckTimer(); }
void RotationState::Left() noexcept { model_->RotateLeft(); }
void RotationState::Right() noexcept { model_->RotateRight(); }
void RotationState::Down() noexcept { model_->RotateDown(); }
void RotationState::Up() noexcept { model_->RotateUp(); }
void RotationState::Action() noexcept { model_->TransitionTo<MovingState>(); }

// MovingState definitions
STATE_CLASS_CONSTRUCT(Moving)
model_->StartTimer();
}

void MovingState::Update() noexcept { model_->MoveSnake(); }
MovingState::~MovingState() noexcept { model_->StartTimer(); }

// ExitState definitions
STATE_CLASS_CONSTRUCT(Exit)
model_->DeleteGameInfo();
}

// GameOverState definitions
STATE_CLASS_CONSTRUCT(GameOver)
model_->SaveScore();
}
void GameOverState::Update() noexcept { model_->SplashField(); }
void GameOverState::Start() noexcept { model_->RestartGame(); }

// WinState definitions
STATE_CLASS_CONSTRUCT(Win)
model_->SaveScore();
}
void WinState::Update() noexcept { model_->SplashField(); }
void WinState::Start() noexcept { model_->RestartGame(); }

}  // namespace s21::snake

#pragma once
#include <functional>
#include <memory>
#include <utility>
#include <vector>

#include "defines.h"
#include "lib.h"

namespace s21::snake {

class State;

class model {
 public:
  class instance {
   public:
    template <class T = Start_state>
    static model* get() {
      static model fsm_ptr = model(std::make_unique<T>());
      return &fsm_ptr;
    }
  };

  friend class instance;

 private:
  model(std::unique_ptr<State> state = nullptr) : state_(std::move(state)) {}

 public:
  template <typename T>
  void TransitionTo() {
    if (state_ != nullptr) state_->Exit();
    state_ = std::make_unique<T>();
    state_->SetFsm(this);
    state_->Enter();
  }

  GameInfo_t updateState() {
    state_->Update();
    return *game_info_;
  };

  void userAction(UserAction_t action) {
    if (state_ != nullptr) state_->Exit();
    const std::unique_ptr<std::vector<std::function<void()>>> actionMap{
        new std::vector<std::function<void()>>{
            [this] { state_->Start(); },
            [this] { state_->Pause(); },
            [this] { state_->Terminate(); },
            [this] { state_->Left(); },
            [this] { state_->Right(); },
            [this] { state_->Down(); },
            [this] { state_->Up(); },
            [this] { state_->Action(); },
        }};
    if (game_info_->pause) TogglePause();
    if (actionMap->size() > action) (*actionMap)[action]();
  };

 private:
  std::unique_ptr<State> state_;
  std::unique_ptr<GameInfo_t> game_info_;

 public:
  void TogglePause() { game_info_->pause = !game_info_->pause; }
  void Exit() { TransitionTo<Exit_state>(); }
  void RotateLeft() {};
  void RotateRight() {};
  void RotateUp() {};
  void RotateDown() {};
  void SpawnSnake() {};
  void SpawnApple() {};
  void MoveSnake() {};
  void StartTimer() {};
  void CheckTimer() {};
};

class State {
 public:
  virtual ~State() = default;
  void SetFsm(model* fsm) { fsm_ = fsm; }
  virtual void Enter() {};
  virtual void Exit() {};
  virtual void Update() {};
  virtual void Start() {};
  virtual void Pause() { fsm_->TogglePause(); };
  virtual void Terminate() { fsm_->Exit(); };
  virtual void Left() {};
  virtual void Right() {};
  virtual void Down() {};
  virtual void Up() {};
  virtual void Action() {};

 protected:
  model* fsm_{nullptr};
};

struct Start_state : public State {
  void Start() override { fsm_->TransitionTo<Spawn_state>(); }
  void Exit() override { fsm_->SpawnSnake(); }
};

struct Spawn_state : public State {
  void Enter() override { fsm_->SpawnApple(); }
  void Update() override { fsm_->TransitionTo<Rotation_state>(); }
};

struct Rotation_state : public State {
  void Enter() override { fsm_->StartTimer(); }
  void Update() override { fsm_->CheckTimer(); }
  void Left() override { fsm_->RotateLeft(); }
  void Right() override { fsm_->RotateRight(); }
  void Down() override { fsm_->RotateDown(); }
  void Up() override { fsm_->RotateUp(); }
  void Down() override { fsm_->RotateDown(); }
};

struct Exit_state : public State {
  void Enter() override { fsm_->Exit(); }
};

};  // namespace s21::snake
#pragma once
#ifndef MODEL_H
#define MODEL_H

#include <functional>
#include <memory>
#include <utility>
#include <vector>

#include "defines.h"
#include "lib.h"

namespace s21::snake {

class model;
class State {
 public:
  virtual ~State() = default;
  void SetFsm(model* fsm) { fsm_ = fsm; }
  virtual void Enter(){};
  virtual void Exit(){};
  virtual void Update(){};
  virtual void Start(){};
  virtual void Pause();
  virtual void Terminate();
  virtual void Left(){};
  virtual void Right(){};
  virtual void Down(){};
  virtual void Up(){};
  virtual void Action(){};

 protected:
  model* fsm_{nullptr};
};

struct Start_state;
struct Spawn_state;
struct Rotation_state;
struct Moving_state;
struct Exit_state;

class model {
  using GI_unique_ptr = std::unique_ptr<GameInfo_t>;

 public:
  class instance {
   public:
    template <class T = Start_state>
    static model* get() {
      static model fsm_ptr = model();
      return &fsm_ptr;
    }
  };

  friend class instance;

 private:
  model();

 public:
  template <typename T>
  void TransitionTo() {
    if (state_.get() != nullptr) state_->Exit();
    state_ = std::make_unique<T>();
    state_->SetFsm(this);
    state_->Enter();
  }

  GameInfo_t updateState() {
    state_->Update();
    return *game_info_;
  };

  void userAction(UserAction_t action) {
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
    if (actionMap->size() > action) (*actionMap)[action]();
  };

 private:
  std::unique_ptr<State> state_;
  GI_unique_ptr game_info_;

 public:
  void TogglePause();
  void InitGI();
  void DeleteGI();
  void RotateLeft(){};
  void RotateRight(){};
  void RotateUp(){};
  void RotateDown(){};
  void SpawnSnake(){};
  void SpawnApple(){};
  void MoveSnake(){};
  void StartTimer(){};
  void CheckTimer(){};
};

struct Start_state : public State {
  void Enter() override { fsm_->InitGI(); }
  void Update() override { fsm_->TransitionTo<Spawn_state>(); }
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
};

struct Exit_state : public State {
  void Enter() override { fsm_->DeleteGI(); }
};

};  // namespace s21::snake
#endif
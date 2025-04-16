#pragma once
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
  virtual void Enter() = 0;
  virtual void Exit() = 0;
  void Start() {};
  void Pause() {};
  void Terminate() {};
  void Left() {};
  void Right() {};
  void Down() {};
  void Up() {};
  void Action() {};

 protected:
  // to allow state changes
  model* fsm_{nullptr};
};

class model {
 public:
  class instance {
   public:
    template <class T = Start_state>
    static std::unique_ptr<model> get() {
      static auto fsm_ptr = new model(std::make_unique<T>());
      return std::unique_ptr<model>(fsm_ptr);
    };
  };

  friend class instance;

 private:
  model(std::unique_ptr<State> state = nullptr) : state_(std::move(state)) {}

 public:
  template <class T>
  void TransitionTo() {
    if (state_ != nullptr) {
      // if switching to same state, do nothing
      const auto temp = std::make_unique<T>();
      if (dynamic_cast<T*>(state_.get()) != nullptr) {
        return;
      }
      state_->Exit();
    }
    state_ = std::make_unique<T>();
    state_->SetFsm(this);
    state_->Enter();
  }

  GameInfo_t updateState() {
    if (state_ != nullptr) state_->Enter();
    return *game_info_;
  }

  void userAction(UserAction_t action) {
    if (state_ != nullptr) state_->Exit();
    auto actionMap = new std::vector<std::function<void()>>{
        [this]{state_->Start();}, [this]{state_->Pause();},  [this]{state_->Terminate();},
        [this]{state_->Left();},  [this]{state_->Right();},  [this]{state_->Down();},
        [this] {state_->Up();},    [this]{state_->Action();},
    };

    (actionMap[action])();
  }

 private:
  std::unique_ptr<State> state_;
  std::unique_ptr<GameInfo_t> game_info_;
};

};  // namespace s21::snake
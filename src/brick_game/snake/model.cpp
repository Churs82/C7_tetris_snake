#include "model.h"

namespace s21::snake {

class Start_state : public State {
  using Next_state = Spawn_state;

 public:
  Start_state(){};
  virtual void Enter() override {};
  virtual void Exit() override { fsm_->TransitionTo<Next_state>(); };
};

class Spawn_state : public State {
  using Next_state = Rotating_state;

 public:
  Spawn_state(){};
  virtual void Enter() override {

  };

  virtual void Exit() override { fsm_->TransitionTo<Next_state>(); };
};

class Exit_state : public State {
 public:
  Exit_state(){};
  virtual void Enter() override {};
  virtual void Exit() override {};
};
}  // namespace s21::snake

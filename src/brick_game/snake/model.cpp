#include "model.h"

namespace s21::snake {

class Start_state : public State {
  using Next_state = Spawn_state;

 public:
  Start_state(){};
  virtual void Enter() override {
    game_info.pause = false;
    game_info.score = 0;
    game_info.high_score = 0;
    game_info.level = 1;
    game_info.speed = 1;
    game_info.field = nullptr;
    game_info.next = nullptr;
  };
  virtual void Exit() override { fsm_->TransitionTo<Next_state>(); };
};

class Exit_state : public State {
 public:
  Exit_state(){};
  virtual void Enter() override{};
  virtual void Exit() override{};
};
}  // namespace s21::snake

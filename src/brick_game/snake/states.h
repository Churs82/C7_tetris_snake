#ifndef SNAKE_STATES_H
#define SNAKE_STATES_H

#pragma once

#include <memory>

#include "defines.h"
#include "lib.h"

namespace s21::snake {

class model;
class State {
 public:
  State(){};
  State(model* fsm) : fsm_(fsm){};
  virtual void Update() {};
  virtual void Start() {};
  virtual void Pause();
  virtual void Terminate();
  virtual void Left() {};
  virtual void Right() {};
  virtual void Down() {};
  virtual void Up() {};
  virtual void Action() {};
  virtual ~State(){};

 protected:
  model* fsm_{nullptr};
};
struct Start_state;
struct Spawn_state;
struct Rotation_state;
struct Moving_state;
struct Exit_state;
struct GameOver_state;
struct Win_state;
}  // namespace s21

#endif  // SNAKE_STATES_H

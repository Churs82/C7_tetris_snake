#ifndef SNAKE_STATES_H
#define SNAKE_STATES_H

#pragma once

#include <memory>

#include "defines.h"
#include "lib.h"
#include "model.h"

namespace s21::snake {

class model;

class State {
 public:
  State();
  explicit State(model* fsm);
  virtual void Update();
  virtual void Start();
  virtual void Pause();
  virtual void Terminate();
  virtual void Left();
  virtual void Right();
  virtual void Down();
  virtual void Up();
  virtual void Action();
  virtual ~State();

 protected:
  model* fsm_{nullptr};
};

struct Start_state : public State {
  explicit Start_state(model* fsm);
  void Update() override;
  ~Start_state();
};

struct Spawn_state : public State {
  explicit Spawn_state(model* fsm);
  void Update() override;
};

struct Rotation_state : public State {
  explicit Rotation_state(model* fsm);
  void Update() override;
  void Left() override;
  void Right() override;
  void Down() override;
  void Up() override;
  void Action() override;
};

struct Moving_state : public State {
  explicit Moving_state(model* fsm);
  void Update() override;
};

struct Exit_state : public State {
  explicit Exit_state(model* fsm);
};

struct GameOver_state : public State {
  explicit GameOver_state(model* fsm);
  void Update() override;
  void Start() override;
};

struct Win_state : public State {
  explicit Win_state(model* fsm);
  void Update() override;
  void Start() override;
};

}  // namespace s21::snake

#endif  // SNAKE_STATES_H

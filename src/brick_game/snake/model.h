#pragma once
#ifndef MODEL_H
#define MODEL_H

#include <chrono>
#include <cstdlib>
#include <fstream>
#include <functional>
#include <memory>
#include <utility>
#include <vector>

#include "defines.h"
#include "lib.h"
#include "states.h"

namespace s21::snake {

class State;

using std::chrono::duration_cast;
using std::chrono::milliseconds;
using std::chrono::steady_clock;
using std::chrono::time_point;

class model {
  // using GI_unique_ptr = std::unique_ptr<GameInfo_t, std::function<void()>>;
  using GI_unique_ptr = std::unique_ptr<GameInfo_t>;

 public:
  /** Singleton */
  struct instance {
    static model* get() {
      static std::unique_ptr<model> fsm_ptr(new model());
      return fsm_ptr.get();
    }
  };
  friend class instance;

 private:
  model();
  std::array<int, 2> s_head{0};
  std::array<int, 2> s_tail{0};
  State* state_{nullptr};
  GI_unique_ptr game_info_{nullptr};
  time_point<steady_clock> timer_;

 public:
  ~model();
  template <typename T>
  void TransitionTo() {
    delete state_;
    state_ = new T(this);
  }

  GameInfo_t UpdateState();

  void UserAction(UserAction_t action);

  /* Game logical functions */
 public:
  void TogglePause();
  void InitGI();
  void DeleteGI();
  void RotateLeft();
  void RotateRight();
  void RotateUp();
  void RotateDown();
  void SpawnSnake();
  void SpawnApple();
  void MoveSnake();
  void StartTimer();
  void CheckTimer();
  void SplashField();
  void LoadScore();
  void SaveScore();

 private:
  void MoveTail();
  void SetDirection(int direction);
  bool CheckBounds(std::array<int, 2> next);
  void AddScore(int s_num = 1);
};
};  // namespace s21::snake
#endif
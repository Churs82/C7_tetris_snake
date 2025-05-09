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

#include "../../inc/lib.h"
#include "defines.h"
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
    /**
     * @brief Get the singleton instance of the model.
     * @return Pointer to the singleton instance.
     */
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

  /**
   * @brief Transition to a new state.
   * @tparam T The type of the new state.
   */
  template <typename T>
  void TransitionTo() {
    delete state_;
    state_ = new T(this);
  }

  /**
   * @brief Update the current state of the game.
   * @return The updated game information.
   */
  GameInfo_t UpdateState();

  /**
   * @brief Handle user actions.
   * @param action The user action to process.
   */
  void UserAction(UserAction_t action);

  /* Game logical functions */
 public:
  /**
   * @brief Toggle the pause state of the game.
   */
  void TogglePause();

  /**
   * @brief Initialize the game information.
   */
  void InitGI();

  /**
   * @brief Delete the game information.
   */
  void DeleteGI();

  /**
   * @brief Rotate the snake to the left.
   */
  void RotateLeft();

  /**
   * @brief Rotate the snake to the right.
   */
  void RotateRight();

  /**
   * @brief Rotate the snake upwards.
   */
  void RotateUp();

  /**
   * @brief Rotate the snake downwards.
   */
  void RotateDown();

  /**
   * @brief Spawn the snake at the starting position.
   */
  void SpawnSnake();

  /**
   * @brief Spawn an apple on the game field.
   */
  void SpawnApple();

  /**
   * @brief Move the snake in the current direction.
   */
  void MoveSnake();

  /**
   * @brief Start the game timer.
   */
  void StartTimer();

  /**
   * @brief Check the game timer for updates.
   */
  void CheckTimer();

  /**
   * @brief Display a splash effect on the game field.
   */
  void SplashField();

  /**
   * @brief Load the player's score from storage.
   */
  void LoadScore();

  /**
   * @brief Save the player's score to storage.
   */
  void SaveScore();

 private:
  /**
   * @brief Move the snake's tail to the next position.
   */
  void MoveTail();

  /**
   * @brief Set the direction of the snake's movement.
   * @param direction The new direction to set.
   */
  void SetDirection(int direction);

  /**
   * @brief Check if the next position is within bounds.
   * @param next The next position to check.
   * @return True if the position is within bounds, false otherwise.
   */
  bool CheckBounds(std::array<int, 2> next);

  /**
   * @brief Add score to the player's total.
   * @param s_num The amount of score to add (default is 1).
   */
  void AddScore(int s_num = 1);
};

};  // namespace s21::snake
#endif
#pragma once
#ifndef MODEL_H
#define MODEL_H

#include <chrono>
#include <cstdlib>
#include <fstream>
#include <functional>
#include <memory>
#include <type_traits>
#include <utility>
#include <vector>

#include "defines.h"
#include "lib.h"

namespace s21::snake {

class State;

using std::chrono::duration_cast;
using std::chrono::milliseconds;
using std::chrono::steady_clock;
using std::chrono::time_point;

/**
 * @brief The Model class manages the game state and logic for the Snake game.
 */
class Model {
  using GiUniquePtr = std::unique_ptr<GameInfo_t>;

 public:
  /**
   * @brief Singleton instance accessor for Model.
   */
  struct Instance {
    /**
     * @brief Returns the singleton instance of Model.
     * @return Pointer to the Model instance.
     */
    static Model* Get() {
      static Model fsm_;
      return &fsm_;
    }
  };

  friend class Instance;

 private:
  /**
   * @brief Constructs a Model object (private for singleton pattern).
   */
  Model() noexcept;
  /**
   * @brief Destructor for Model.
   */
  ~Model() noexcept;

  std::array<int, 2> snake_head_{0}; /**< Coordinates of the snake's head. */
  std::array<int, 2> snake_tail_{0}; /**< Coordinates of the snake's tail. */
  State* state_; /**< Pointer to the current state object. */
  GiUniquePtr game_info_ =
      nullptr; /**< Unique pointer to the game info struct. */
  time_point<steady_clock> timer_; /**< Timer for game events. */

 public:
  /**
   * @brief Transitions to a new state of type T.
   * @tparam T The state class to transition to.
   */
  template <class T>
  void TransitionTo() {
    // If the current state is not of type T, delete it and create a new one.
    if (std::is_base_of<State, T>::value && !dynamic_cast<T*>(state_)) {
      delete state_;
      state_ = new T(this);
    }
  }

  /**
   * @brief Updates the current game state.
   * @return The updated GameInfo_t struct.
   */
  GameInfo_t UpdateState() noexcept;

  /**
   * @brief Handles a user action.
   * @param action The user action to process.
   */
  void UserAction(const UserAction_t action) noexcept;
#ifndef IM_STATE
 private:
#endif
  /**
   * @brief Toggles the pause state of the game.
   */
  void TogglePause() noexcept;

  /**
   * @brief Initializes the game info structure.
   */
  void InitGameInfo() noexcept;

  /**
   * @brief Deletes the game info structure and frees resources.
   */
  void DeleteGameInfo() noexcept;

  /**
   * @brief Rotates the snake left.
   */
  void RotateLeft() noexcept;

  /**
   * @brief Rotates the snake right.
   */
  void RotateRight() noexcept;

  /**
   * @brief Rotates the snake up.
   */
  void RotateUp() noexcept;

  /**
   * @brief Rotates the snake down.
   */
  void RotateDown() noexcept;

  /**
   * @brief Spawns the snake on the field.
   */
  void SpawnSnake() noexcept;

  /**
   * @brief Spawns an apple on the field.
   */
  void SpawnApple() noexcept;

  /**
   * @brief Moves the snake according to the current direction.
   */
  void MoveSnake() noexcept;

  /**
   * @brief Starts the game timer.
   */
  void StartTimer() noexcept;

  /**
   * @brief Checks the timer and triggers actions if needed.
   */
  void CheckTimer() noexcept;

  /**
   * @brief Performs a splash animation on the field.
   */
  void SplashField() noexcept;

  /**
   * @brief Loads the high score from persistent storage.
   */
  void LoadScore() noexcept;

  /**
   * @brief Saves the high score to persistent storage.
   */
  void SaveScore() noexcept;

  /**
   * @brief Restarts the game.
   */
  void RestartGame() noexcept;

 private:
  /**
   * @brief Moves the snake's tail.
   */
  void MoveTail() noexcept;

  /**
   * @brief Sets the snake's direction.
   * @param direction The direction to set.
   */
  void SetDirection(const int direction) noexcept;

  /**
   * @brief Checks if the next position is within bounds.
   * @param next The next position to check.
   * @return True if within bounds, false otherwise.
   */
  bool CheckBounds(const std::array<int, 2>& next) const noexcept;

  /**
   * @brief Adds score to the current game.
   * @param score_num The amount to add (default 1).
   */
  void AddScore(const int score_num = 1) noexcept;
};

};  // namespace s21::snake
#endif
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
  Model();
  std::array<int, 2> snake_head_{}; /**< Coordinates of the snake's head. */
  std::array<int, 2> snake_tail_{}; /**< Coordinates of the snake's tail. */
  State* state_ = nullptr;          /**< Pointer to the current state object. */
  GiUniquePtr game_info_ =
      nullptr; /**< Unique pointer to the game info struct. */
  time_point<steady_clock> timer_; /**< Timer for game events. */

 public:
  /**
   * @brief Destructor for Model.
   */
  ~Model();

  /**
   * @brief Transitions to a new state of type T.
   * @tparam T The state class to transition to.
   */
  template <typename T>
  void TransitionTo() {
    delete state_;
    state_ = new T(this);
  }

  /**
   * @brief Updates the current game state.
   * @return The updated GameInfo_t struct.
   */
  GameInfo_t UpdateState();

  /**
   * @brief Handles a user action.
   * @param action The user action to process.
   */
  void UserAction(UserAction_t action);

  /**
   * @brief Toggles the pause state of the game.
   */
  void TogglePause();

  /**
   * @brief Initializes the game info structure.
   */
  void InitGameInfo();

  /**
   * @brief Deletes the game info structure and frees resources.
   */
  void DeleteGameInfo();

  /**
   * @brief Rotates the snake left.
   */
  void RotateLeft();

  /**
   * @brief Rotates the snake right.
   */
  void RotateRight();

  /**
   * @brief Rotates the snake up.
   */
  void RotateUp();

  /**
   * @brief Rotates the snake down.
   */
  void RotateDown();

  /**
   * @brief Spawns the snake on the field.
   */
  void SpawnSnake();

  /**
   * @brief Spawns an apple on the field.
   */
  void SpawnApple();

  /**
   * @brief Moves the snake according to the current direction.
   */
  void MoveSnake();

  /**
   * @brief Starts the game timer.
   */
  void StartTimer();

  /**
   * @brief Checks the timer and triggers actions if needed.
   */
  void CheckTimer();

  /**
   * @brief Performs a splash animation on the field.
   */
  void SplashField();

  /**
   * @brief Loads the high score from persistent storage.
   */
  void LoadScore();

  /**
   * @brief Saves the high score to persistent storage.
   */
  void SaveScore();

  /**
   * @brief Restarts the game.
   */
  void RestartGame();

 private:
  /**
   * @brief Moves the snake's tail.
   */
  void MoveTail();

  /**
   * @brief Sets the snake's direction.
   * @param direction The direction to set.
   */
  void SetDirection(int direction);

  /**
   * @brief Checks if the next position is within bounds.
   * @param next The next position to check.
   * @return True if within bounds, false otherwise.
   */
  bool CheckBounds(std::array<int, 2> next) const noexcept;

  /**
   * @brief Adds score to the current game.
   * @param score_num The amount to add (default 1).
   */
  void AddScore(int score_num = 1);
};

};  // namespace s21::snake
#endif
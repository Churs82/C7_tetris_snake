#ifndef SNAKE_STATES_H
#define SNAKE_STATES_H

#pragma once

#include <memory>

#include "defines.h"
#include "lib.h"
#include "model.h"

namespace s21::snake {
#define STATE_CLASS(name)            \
  class name##State : public State { \
   public:                           \
    name##State(Model* model) noexcept;

#define STATE_CLASS_CONSTRUCT(name) \
  name##State::name##State(Model* model) noexcept : State(model) {
/**
 * @brief Base class for all game states.
 */
class State {
 protected:
  Model* const model_; /**< Pointer to the Model instance. */

 public:
  /**
   * @brief List of actions mapped to corresponding state functions.
   */
  const std::vector<std::function<void()>> action_map{
      [this] { Start(); }, [this] { Pause(); },  [this] { Terminate(); },
      [this] { Left(); },  [this] { Right(); },  [this] { Up(); },
      [this] { Down(); },  [this] { Action(); },
  };
  State(Model* model) noexcept : model_(model) {}

  /**
   * @brief Default constructor for the State class.
   */
  State() = default;

  /**
   * @brief Virtual function to update the state.
   */
  virtual void Update() noexcept {};

  /**
   * @brief Virtual function to handle the start action.
   */
  virtual void Start() noexcept {};

  /**
   * @brief Virtual function to handle the pause action.
   */
  void Pause() noexcept;

  /**
   * @brief Virtual function to handle the terminate action.
   */
  void Terminate() noexcept;

  /**
   * @brief Virtual function to handle the left action.
   */
  virtual void Left() noexcept {};

  /**
   * @brief Virtual function to handle the right action.
   */
  virtual void Right() noexcept {};

  /**
   * @brief Virtual function to handle the down action.
   */
  virtual void Down() noexcept {};

  /**
   * @brief Virtual function to handle the up action.
   */
  virtual void Up() noexcept {};

  /**
   * @brief Virtual function to handle the action event.
   */
  virtual void Action() noexcept {};

  /**
   * @brief Virtual destructor for the State class.
   */
  virtual ~State() noexcept = default;
};

/**
 * @brief State representing the start of the game.
 */
STATE_CLASS(Start)

/**
 * @brief Handle the start signal action in the StartState.
 */
void Start() noexcept override;
};

/**
 * @brief State representing the spawning of game elements.
 */
STATE_CLASS(Spawn)

/**
 * @brief Update function for the SpawnState.
 */
void Update() noexcept override;
}
;

/**
 * @brief State representing the rotation of the snake.
 */
STATE_CLASS(Rotation)

/**
 * @brief Update function for the RotationState.
 */
void Update() noexcept override;

/**
 * @brief Handle the left rotation action.
 */
void Left() noexcept override;

/**
 * @brief Handle the right rotation action.
 */
void Right() noexcept override;

/**
 * @brief Handle the down rotation action.
 */
void Down() noexcept override;

/**
 * @brief Handle the up rotation action.
 */
void Up() noexcept override;

/**
 * @brief Handle the action event.
 */
void Action() noexcept override;
}
;

STATE_CLASS(Moving)

/**
 * @brief Update function for the MovingState.
 */
virtual void Update() noexcept override;

/**
 * @brief Destructor to reset the timer after moving
 */
~MovingState() noexcept override;
}
;

/**
 * @brief State representing the exit of the game.
 */
STATE_CLASS(Exit)
}
;

/**
 * @brief State representing the game over condition.
 */
STATE_CLASS(GameOver)

/**
 * @brief Update function for the GameOverState.
 */
void Update() noexcept override;

/**
 * @brief Handle the start action in the GameOverState.
 */
void Start() noexcept override;
}
;

/**
 * @brief State representing the win condition.
 */
STATE_CLASS(Win)

/**
 * @brief Update function for the WinState.
 */
void Update() noexcept override;

/**
 * @brief Handle the start action in the WinState.
 */
void Start() noexcept override;
}
;

}  // namespace s21::snake

#endif  // SNAKE_STATES_H

#ifndef SNAKE_STATES_H
#define SNAKE_STATES_H

#pragma once

#include <memory>

#include "../../inc/lib.h"
#include "defines.h"
#include "model.h"

namespace s21::snake {

/**
 * @brief Forward declaration of the model class.
 */
class Model;

/**
 * @brief Base class for all game states.
 */
class State {
 public:
  /**
   * @brief List of actions mapped to corresponding state functions.
   */
  const std::vector<std::function<void()>> action_map{
      [this] { Start(); }, [this] { Pause(); },  [this] { Terminate(); },
      [this] { Left(); },  [this] { Right(); },  [this] { Up(); },
      [this] { Down(); },  [this] { Action(); },
  };

  /**
   * @brief Default constructor for the State class.
   */
  State();

  /**
   * @brief Constructor for the State class with a model pointer.
   * @param model Pointer to the model instance.
   */
  explicit State(Model* model);

  /**
   * @brief Virtual function to update the state.
   */
  virtual void Update() {};

  /**
   * @brief Virtual function to handle the start action.
   */
  void virtual Start(){};

  /**
   * @brief Virtual function to handle the pause action.
   */
  void Pause();

  /**
   * @brief Virtual function to handle the terminate action.
   */
  void Terminate();

  /**
   * @brief Virtual function to handle the left action.
   */
  virtual void Left() {};

  /**
   * @brief Virtual function to handle the right action.
   */
  virtual void Right() {};

  /**
   * @brief Virtual function to handle the down action.
   */
  virtual void Down() {};

  /**
   * @brief Virtual function to handle the up action.
   */
  virtual void Up() {};

  /**
   * @brief Virtual function to handle the action event.
   */
  virtual void Action() {};

  /**
   * @brief Virtual destructor for the State class.
   */
  virtual ~State() = default;

 protected:
  /**
   * @brief Pointer to the model instance.
   */
  Model* model_ = nullptr;
};

/**
 * @brief State representing the start of the game.
 */
struct StartState : public State {
  /**
   * @brief Constructor for the StartState class.
   * @param model Pointer to the model instance.
   */
  explicit StartState(Model* model);

  /**
   * @brief Handle the start signal action in the StartState.
   */
  void Start() override;
  /**
   * @brief Destructor for the StartState class.
   */
  ~StartState();
};

/**
 * @brief State representing the spawning of game elements.
 */
struct SpawnState : public State {
  /**
   * @brief Constructor for the SpawnState class.
   * @param model Pointer to the model instance.
   */
  explicit SpawnState(Model* model);

  /**
   * @brief Update function for the SpawnState.
   */
  void Update() override;
};

/**
 * @brief State representing the rotation of the snake.
 */
struct RotationState : public State {
  /**
   * @brief Constructor for the RotationState class.
   * @param model Pointer to the model instance.
   */
  explicit RotationState(Model* model);

  /**
   * @brief Update function for the RotationState.
   */
  void Update() override;

  /**
   * @brief Handle the left rotation action.
   */
  void Left() override;

  /**
   * @brief Handle the right rotation action.
   */
  void Right() override;

  /**
   * @brief Handle the down rotation action.
   */
  void Down() override;

  /**
   * @brief Handle the up rotation action.
   */
  void Up() override;

  /**
   * @brief Handle the action event.
   */
  void Action() override;
};

/**
 * @brief State representing the movement of the snake.
 */
struct MovingState : public State {
  /**
   * @brief Constructor for the MovingState class.
   * @param model Pointer to the model instance.
   */
  explicit MovingState(Model* model);

  /**
   * @brief Update function for the MovingState.
   */
  void Update() override;

  /**
   * @brief Destructor to reset the timer after moving
   */
  ~MovingState() override;
};

/**
 * @brief State representing the exit of the game.
 */
struct ExitState : public State {
  /**
   * @brief Constructor for the ExitState class.
   * @param model Pointer to the model instance.
   */
  explicit ExitState(Model* model);

  void Update() override;
};

/**
 * @brief State representing the game over condition.
 */
struct GameOverState : public State {
  /**
   * @brief Constructor for the GameOverState class.
   * @param model Pointer to the model instance.
   */
  explicit GameOverState(Model* model);

  /**
   * @brief Update function for the GameOverState.
   */
  void Update() override;

  /**
   * @brief Handle the start action in the GameOverState.
   */
  void Start() override;
};

/**
 * @brief State representing the win condition.
 */
struct WinState : public State {
  /**
   * @brief Constructor for the WinState class.
   * @param model Pointer to the model instance.
   */
  explicit WinState(Model* model);

  /**
   * @brief Update function for the WinState.
   */
  void Update() override;

  /**
   * @brief Handle the start action in the WinState.
   */
  void Start() override;
};

}  // namespace s21::snake

#endif  // SNAKE_STATES_H

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
class model;

/**
 * @brief Base class for all game states.
 */
class State {
 public:
  /**
   * @brief List of actions mapped to corresponding state functions.
   */
  const std::vector<std::function<void()>> actionMap{
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
   * @param fsm Pointer to the model instance.
   */
  explicit State(model* fsm);

  /**
   * @brief Virtual function to update the state.
   */
  virtual void Update();

  /**
   * @brief Virtual function to handle the start action.
   */
  virtual void Start();

  /**
   * @brief Virtual function to handle the pause action.
   */
  virtual void Pause();

  /**
   * @brief Virtual function to handle the terminate action.
   */
  virtual void Terminate();

  /**
   * @brief Virtual function to handle the left action.
   */
  virtual void Left();

  /**
   * @brief Virtual function to handle the right action.
   */
  virtual void Right();

  /**
   * @brief Virtual function to handle the down action.
   */
  virtual void Down();

  /**
   * @brief Virtual function to handle the up action.
   */
  virtual void Up();

  /**
   * @brief Virtual function to handle the action event.
   */
  virtual void Action();

  /**
   * @brief Virtual destructor for the State class.
   */
  virtual ~State();

 protected:
  /**
   * @brief Pointer to the model instance.
   */
  model* fsm_{nullptr};
};

/**
 * @brief State representing the start of the game.
 */
struct Start_state : public State {
  /**
   * @brief Constructor for the Start_state class.
   * @param fsm Pointer to the model instance.
   */
  explicit Start_state(model* fsm);

  /**
   * @brief Update function for the Start_state.
   */
  void Update() override;

  /**
   * @brief Destructor for the Start_state class.
   */
  ~Start_state();
};

/**
 * @brief State representing the spawning of game elements.
 */
struct Spawn_state : public State {
  /**
   * @brief Constructor for the Spawn_state class.
   * @param fsm Pointer to the model instance.
   */
  explicit Spawn_state(model* fsm);

  /**
   * @brief Update function for the Spawn_state.
   */
  void Update() override;
};

/**
 * @brief State representing the rotation of the snake.
 */
struct Rotation_state : public State {
  /**
   * @brief Constructor for the Rotation_state class.
   * @param fsm Pointer to the model instance.
   */
  explicit Rotation_state(model* fsm);

  /**
   * @brief Update function for the Rotation_state.
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
struct Moving_state : public State {
  /**
   * @brief Constructor for the Moving_state class.
   * @param fsm Pointer to the model instance.
   */
  explicit Moving_state(model* fsm);

  /**
   * @brief Update function for the Moving_state.
   */
  void Update() override;
};

/**
 * @brief State representing the exit of the game.
 */
struct Exit_state : public State {
  /**
   * @brief Constructor for the Exit_state class.
   * @param fsm Pointer to the model instance.
   */
  explicit Exit_state(model* fsm);

  void Update() override;
};

/**
 * @brief State representing the game over condition.
 */
struct GameOver_state : public State {
  /**
   * @brief Constructor for the GameOver_state class.
   * @param fsm Pointer to the model instance.
   */
  explicit GameOver_state(model* fsm);

  /**
   * @brief Update function for the GameOver_state.
   */
  void Update() override;

  /**
   * @brief Handle the start action in the GameOver_state.
   */
  void Start() override;
};

/**
 * @brief State representing the win condition.
 */
struct Win_state : public State {
  /**
   * @brief Constructor for the Win_state class.
   * @param fsm Pointer to the model instance.
   */
  explicit Win_state(model* fsm);

  /**
   * @brief Update function for the Win_state.
   */
  void Update() override;

  /**
   * @brief Handle the start action in the Win_state.
   */
  void Start() override;
};

}  // namespace s21::snake

#endif  // SNAKE_STATES_H

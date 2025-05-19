#ifndef LIB_H
#define LIB_H

#include <stdbool.h>

#define ROWS_MAP 20
#define COLS_MAP 10

#define SUCCESS 0
#define ERROR 1
#define FIGURE_W 4
#define FIGURE_H 4

#define DIRECTION_MASK (3 << 6)
#define HEAD_MASK (1 << 8)
#define TAIL_MASK (1 << 9)
#define SNAKE_MASK 1
#define APPLE_MASK 2

#define INTRO_MESSAGE "Press ENTER to start or Esc to exit!"
#define EXIT_MESSAGE "Press ENTER to resume or ESC to exit!"
#define PAUSE_MESSAGE "Press any key to resume!"
#ifdef __cplusplus
extern "C" {
#endif
#ifdef GAME
#define G_TITLE GAME
#else
#define G_TITLE "Brick Game"
#endif

/**
 * @brief Enum representing possible user actions in the game.
 */
typedef enum {
  Start,     /**< Start the game or resume. */
  Pause,     /**< Pause the game. */
  Terminate, /**< Terminate or exit the game. */
  Left,      /**< Move or rotate left. */
  Right,     /**< Move or rotate right. */
  Up,        /**< Move or rotate up. */
  Down,      /**< Move or rotate down. */
  Action     /**< Perform an action (e.g., confirm, drop, etc.). */
} UserAction_t;

/**
 * @brief Struct holding the current state of the game.
 */
typedef struct {
  int **field;    /**< Pointer to the game field array. */
  int **next;     /**< Pointer to the next figure/element array. */
  int score;      /**< Current score. */
  int high_score; /**< High score. */
  int level;      /**< Current level. */
  int speed;      /**< Current speed. */
  int pause;      /**< Pause state (0 = running, 1 = paused). */
} GameInfo_t;

/**
 * @brief Sends a user action to the game logic.
 * @param action The user action to send.
 * @param hold Whether the action is being held (true) or pressed once (false).
 */
void userInput(UserAction_t action, bool hold);

/**
 * @brief Gets the current state of the game.
 * @return The current GameInfo_t struct.
 */
GameInfo_t updateCurrentState();
#ifdef __cplusplus
}
#endif
#endif
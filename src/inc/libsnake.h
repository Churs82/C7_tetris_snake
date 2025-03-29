#ifndef SNAKE_LIB_H
#define SNAKE_LIB_H

#include <stdbool.h>

#define ROWS_MAP 20
#define COLS_MAP 10

#define SUCCESS 0
#define ERROR 1

#define FIGURE_W 4
#define FIGURE_H 4

#define DIRECTION_MASK 192
#define HEAD_MASK 256

#define INTRO_MESSAGE                                                      \
  " Press ENTER to start! ***********************        Controls:       " \
  "arrows - move,         spacebar - speedup,     ESC - exit, p - pause  "
#define EXIT_MESSAGE " Press ENTER to resume  or ESC to exit!"
#define GAMEOVER_MESSAGE "Press ENTER to restart or ESC to exit!"

typedef enum {
  Start,
  Pause,
  Terminate,
  Left,
  Right,
  Up,
  Down,
  Action
} UserAction_t;

typedef struct {
  int **field;
  int **next;
  int score;
  int high_score;
  int level;
  int speed;
  int pause;
} GameInfo_t;

typedef enum {
  START = 0,
  SPAWN,
  MOVING,
  DOWN_SHIFTING,
  ATTACHING,
  GAME_OVER,
  EXIT_STATE,
} game_state;

void userInput(UserAction_t action, bool hold);

GameInfo_t updateCurrentState();

game_state getState();

#endif
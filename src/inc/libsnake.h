#ifndef SNAKE_LIB_H
#define SNAKE_LIB_H

#include <stdbool.h>

#include "defines.h"

#define INITIAL_SPEED 1
#define INITIAL_LEVEL 1
#define MAX_LEVEL 10
#define INITIAL_LEN 4
#define LEVEL_UP_SCORE 5
#define HS_FILE "~/games/snake_hs.conf"

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
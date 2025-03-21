#ifndef SNAKE_LIB_H
#define SNAKE_LIB_H

#include <stdbool.h>

#include "defines.h"

#define LEVEL_UP_SCORE 600
#define HS_FILE "~/games/snake_hs.conf"

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
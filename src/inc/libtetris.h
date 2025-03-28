#ifndef TETRIS_LIB_H
#define TETRIS_LIB_H

#include <stdbool.h>

#include "defines.h"

#define FIGURE_W 4
#define FIGURE_H 4
#define FIGURES_COUNT 7
#define FIGURES                                                               \
  (int[FIGURE_H * FIGURES_COUNT][FIGURE_W]) {                                 \
    {0, 2, 0, 0}, {0, 2, 0, 0}, {0, 2, 0, 0}, {0, 2, 0, 0}, {0, 0, 0, 0},     \
        {0, 0, 0, 0}, {0, 3, 3, 0}, {0, 3, 3, 0}, {0, 0, 0, 0}, {0, 4, 4, 0}, \
        {0, 4, 0, 0}, {0, 4, 0, 0}, {0, 0, 0, 0}, {0, 5, 5, 0}, {0, 0, 5, 0}, \
        {0, 0, 5, 0}, {0, 0, 0, 0}, {0, 0, 6, 0}, {0, 6, 6, 0}, {0, 6, 0, 0}, \
        {0, 0, 0, 0}, {0, 7, 0, 0}, {0, 7, 7, 0}, {0, 0, 7, 0}, {0, 0, 0, 0}, \
        {0, 0, 0, 0}, {0, 8, 8, 8}, {                                         \
      0, 0, 8, 0                                                              \
    }                                                                         \
  }
#define LEVEL_UP_SCORE 600
#define HS_FILE "~/games/tetris_hs.conf"

#define INTRO_MESSAGE                                                      \
  " Press ENTER to start! ***********************        Controls:       " \
  "arrows - move,         spacebar - rotate,     ESC - exit, p - pause  "
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
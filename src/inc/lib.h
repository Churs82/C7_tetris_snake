#ifndef LIB_H
#define LIB_H

#include <stdbool.h>

#define ROWS_MAP 20
#define COLS_MAP 10

#define SUCCESS 0
#define ERROR 1
#define FIGURE_W 4
#define FIGURE_H 4

#define DIRECTION_MASK 3<<6
#define HEAD_MASK 1<<8

#define INTRO_MESSAGE                                                      \
  " Press ENTER to start or Esc to exit! ***********************"
#define EXIT_MESSAGE " Press ENTER to resume or ESC to exit!"
#define GAMEOVER_MESSAGE "Press ENTER to restart or ESC to exit!"
#define WIN_MESSAGE \
  "You won!!!"      \
  "Press ENTER to restart or ESC to exit!"

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

void userInput(UserAction_t action, bool hold);

GameInfo_t updateCurrentState();

#endif
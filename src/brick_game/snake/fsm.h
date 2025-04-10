#ifndef FSM_H
#define FSM_H

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "defines.h"
#include "lib.h"



typedef enum {
  START = 0,
  SPAWN,
  ROTATING,
  MOVING,
  WIN,
  GAME_OVER,
  EXIT_STATE
} game_state;

void _userAction(UserAction_t action);

GameInfo_t _updateCurrentState();

game_state _getState();

#endif
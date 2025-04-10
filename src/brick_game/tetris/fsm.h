#ifndef FSM_H
#define FSM_H

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "defines.h"
#include "lib.h"

/* STATE\SIGNAL: START PAUSE TERMINATE LEFT RIGHT UP DOWN ACTION */
#define FSM_TABLE                                                     \
  (act_t[EXIT_STATE + 1][Action + 1]) { /* START */                   \
    {spawn_sw, NULL, doexit, NULL, NULL, NULL, NULL, NULL},           \
        /* SPAWN */ {NULL, pausetoggle, exitstate_sw, NULL,           \
                     NULL, NULL,        NULL,         NULL},          \
        /* MOVING */ {NULL,      pausetoggle, exitstate_sw, moveleft, \
                      moveright, NULL,        down,         rotate},  \
        /* DOWNSHIFTING */ {NULL, pausetoggle, exitstate_sw, NULL,    \
                            NULL, NULL,        NULL,         NULL},   \
        /* ATTACHING */ {NULL, pausetoggle, exitstate_sw, NULL, NULL, \
                         NULL, NULL,        NULL}, /* GAME_OVER       \
                                                    */                \
        {restart, NULL, doexit, NULL, NULL, NULL, NULL, NULL},        \
    /* EXIT_STATE */ {                                                \
      move_sw, NULL, doexit, NULL, NULL, NULL, NULL, NULL             \
    }                                                                 \
  }
/* START SPAWN MOVING DOWNSHIFTING ATTACHING GAME_OVER EXIT_STATE */
#define FSM_TRANSFER \
  (act_t[EXIT_STATE + 1]) { NULL, spawn, checkTime, down, attach, NULL, NULL }

typedef void (*act_t)();

typedef enum {
  START = 0,
  SPAWN,
  MOVING,
  DOWN_SHIFTING,
  ATTACHING,
  GAME_OVER,
  EXIT_STATE,
} game_state;

void _userAction(UserAction_t action);

GameInfo_t _updateCurrentState();

game_state _getState();

#endif
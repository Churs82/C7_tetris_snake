/**
 * @file fsm.h
 * @brief Finite State Machine (FSM) interface for Tetris game logic.
 *
 * This header defines the FSM table, state transitions, and core functions for Tetris game logic.
 */
#ifndef FSM_H
#define FSM_H

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "defines.h"
#include "lib.h"
/**
 * @brief FSM table for state transitions and actions.
 *
 * STATE\SIGNAL: START PAUSE TERMINATE LEFT RIGHT UP DOWN ACTION 
 * */
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
      restart, NULL, doexit, NULL, NULL, NULL, NULL, NULL             \
    }                                                                 \
  }
/* START SPAWN MOVING DOWNSHIFTING ATTACHING GAME_OVER EXIT_STATE */
#define FSM_TRANSFER \
  (act_t[EXIT_STATE + 1]) { NULL, spawn, checkTime, down, attach, NULL, doexit }

#ifndef TESTING
#define SCOPE extern
#else
#define SCOPE static
#endif

/**
 * @brief Action function pointer type for FSM actions.
 */
typedef void (*act_t)();

/**
 * @brief Enum representing the possible game states in Tetris.
 */
typedef enum {
  START = 0,   /**< Start state. */
  SPAWN,       /**< Spawn state. */
  MOVING,      /**< Moving state. */
  DOWN_SHIFTING,/**< Down shifting state. */
  ATTACHING,   /**< Attaching state. */
  GAME_OVER,   /**< Game over state. */
  EXIT_STATE,  /**< Exit state. */
} game_state;

/**
 * @brief Handles a user action in the FSM.
 * @param action The user action to process.
 */
void _userAction(UserAction_t action);

/**
 * @brief Updates and returns the current game state info.
 * @return The current GameInfo_t struct.
 */
GameInfo_t _updateCurrentState();

#endif
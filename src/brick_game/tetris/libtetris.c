#include <sys/time.h>
#include <time.h>

#include "fsm.h"
#include "lib.h"

/**
 * @file libtetris.c
 * @brief Tetris game interface implementation for user input and state update.
 *
 * This file provides the userInput and updateCurrentState functions for the
 * Tetris game.
 */

/**
 * @brief Sends a user action to the Tetris game logic.
 * @param action The user action to send.
 * @param hold Whether the action is being held (true) or pressed once (false).
 */
void userInput(UserAction_t action, bool hold) { _userAction(action); }

/**
 * @brief Gets the current state of the Tetris game.
 * @return The current GameInfo_t struct.
 */
GameInfo_t updateCurrentState() { return _updateCurrentState(); }

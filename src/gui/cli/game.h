#ifndef GAME_H
#define GAME_H

#include "../../inc/lib.h"
#include "interface.h"

/**
 * @file game.h
 * @brief CLI game loop and interface for Brick Game Project.
 *
 * This header declares the main game loop and related functions for the CLI
 * version of the Brick Game Project.
 */

/**
 * @brief Runs the main game loop for the CLI interface.
 */
void game_loop();

/**
 * @brief Handles the end-of-game logic and user prompt.
 */
void end_game();

/**
 * @brief Shows the game intro and waits for user input to start or terminate.
 */
void intro();

#endif
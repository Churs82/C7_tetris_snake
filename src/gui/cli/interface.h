/**
 * @file interface.h
 * @brief CLI interface utilities for Brick Game Project.
 *
 * This header provides macros and function declarations for the ncurses-based CLI interface.
 */

#ifndef INTERFACE_H
#define INTERFACE_H

#include <locale.h>
#include <ncurses.h>
#include <string.h>

#include "../../inc/lib.h"
#include "game.h"

#define BOARDS_BEGIN 1
#define BOARD_SCALE 2
#define BOARD_N (ROWS_MAP + MAP_PADDING)
#define BOARD_M (COLS_MAP * BOARD_SCALE + MAP_PADDING * 2)

#define HUD_WIDTH 14
#define MAP_PADDING 2

#define BANNER_N 10
#define BANNER_M 100

#define WIN_INIT(time)    \
  {                       \
    initscr();            \
    noecho();             \
    curs_set(0);          \
    keypad(stdscr, TRUE); \
    timeout(time);        \
  }

#define GET_USER_INPUT getch()

#define INIT_COLORS                          \
  start_color();                             \
  init_pair(0, COLOR_WHITE, COLOR_BLACK);    \
  init_pair(1, COLOR_BLACK, COLOR_WHITE);    \
  init_pair(2, COLOR_BLUE, COLOR_BLACK);     \
  init_pair(3, COLOR_GREEN, COLOR_BLACK);    \
  init_pair(4, COLOR_YELLOW, COLOR_BLACK);   \
  init_pair(5, COLOR_CYAN, COLOR_BLACK);     \
  init_pair(6, COLOR_MAGENTA, COLOR_BLACK);  \
  init_pair(10, COLOR_RED, COLOR_BLACK);     \
  init_pair(7, COLOR_WHITE, COLOR_RED);      \
  init_pair(8, COLOR_WHITE, COLOR_GREEN);    \
  init_pair(9, COLOR_WHITE, COLOR_BLUE);     \
  init_pair(11, COLOR_WHITE, COLOR_MAGENTA); \
  init_pair(12, COLOR_WHITE, COLOR_CYAN);    \
  init_pair(13, COLOR_WHITE, COLOR_YELLOW);  \
  init_pair(14, COLOR_RED, COLOR_WHITE);     \
  init_pair(15, COLOR_GREEN, COLOR_WHITE);   \
  init_pair(16, COLOR_BLUE, COLOR_WHITE);    \
  init_pair(17, COLOR_MAGENTA, COLOR_WHITE); \
  init_pair(18, COLOR_CYAN, COLOR_WHITE);    \
  init_pair(19, COLOR_YELLOW, COLOR_WHITE);

#define COLOR_MASK 63

#define PRINT_OBJ(y, x, c)                                                     \
  for (int scale_iterator = 0; scale_iterator < BOARD_SCALE; scale_iterator++) \
    mvaddch(BOARDS_BEGIN + (y),                                                \
            BOARDS_BEGIN + (x) * BOARD_SCALE - scale_iterator, c);
#define PRINT_CHAR(y, x, c) \
  mvaddch(BOARDS_BEGIN + (y), BOARDS_BEGIN + (x) * BOARD_SCALE - 1, c)

#define BOARD_CLEAN                                               \
  for (int clean_board_y = BOARDS_BEGIN + 1;                      \
       clean_board_y < BOARDS_BEGIN + BOARD_N; clean_board_y++)   \
    for (int clean_board_x = BOARDS_BEGIN + 1;                    \
         clean_board_x < BOARDS_BEGIN + BOARD_M; clean_board_x++) \
      mvaddch(clean_board_y, clean_board_x, ' ' | COLOR_PAIR(0));
#define MVPRINTW(y, x, ...) \
  mvprintw(BOARDS_BEGIN + (y), BOARDS_BEGIN + (x), __VA_ARGS__)
#define MVADDCH(y, x, c) mvaddch(BOARDS_BEGIN + (y), BOARDS_BEGIN + (x), c)
#define CLEAR_BACKPOS(y, x) mvaddch(BOARDS_BEGIN + (y), BOARDS_BEGIN + (x), ' ')

/* INPUT DEFINES*/
#define NO_INPUT -1
#define ESCAPE 27
#define ENTER_KEY 10
#define SPACE_KEY 32
#define PAUSE_KEY 112

// Start,  Pause,  Terminate,  Left,  Right,  Up,  Down,  Action
#define KEYMAP                                                           \
  (int[Action + 1]) {                                                    \
    ENTER_KEY, PAUSE_KEY, ESCAPE, KEY_LEFT, KEY_RIGHT, KEY_UP, KEY_DOWN, \
        SPACE_KEY                                                        \
  }

/**
 * @brief Prints the overlay (HUD and borders) for the game.
 */
void print_overlay(void);

/**
 * @brief Prints a level error message.
 */
void print_levelerror(void);

/**
 * @brief Prints a rectangle on the screen.
 * @param top_y Top y coordinate.
 * @param bottom_y Bottom y coordinate.
 * @param left_x Left x coordinate.
 * @param right_x Right x coordinate.
 */
void print_rectangle(int top_y, int bottom_y, int left_x, int right_x);

/**
 * @brief Prints a message on the screen.
 * @param msg The message to print.
 */
void print_msg(char *msg);

/**
 * @brief Prints the game intro screen.
 */
void print_intro(void);

/**
 * @brief Prints the exit screen.
 */
void print_exit(void);

/**
 * @brief Prints the game over screen.
 */
void print_gameover(void);

/**
 * @brief Prints the game statistics (score, high score, level).
 * @param game_info The current game state.
 */
void print_stats(GameInfo_t game_info);

/**
 * @brief Prints the game board.
 * @param game_info The current game state.
 */
void print_board(GameInfo_t game_info);

/**
 * @brief Translates a user input key into a UserAction_t.
 * @param user_input The input key.
 * @return The corresponding UserAction_t.
 */
UserAction_t get_signal(int user_input);

#endif  // INTERFACE_H
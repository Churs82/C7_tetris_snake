#include "../inc/interface.h"

void print_overlay(void) {
  INIT_COLORS;
  attron(COLOR_PAIR(0));
  print_rectangle(0, BOARD_N, 0, BOARD_M);
  print_rectangle(0, BOARD_N, BOARD_M + 2, BOARD_M + HUD_WIDTH + 3);

  print_rectangle(1, 4, BOARD_M + 3, BOARD_M + HUD_WIDTH + 2);
  print_rectangle(5, 8, BOARD_M + 3, BOARD_M + HUD_WIDTH + 2);
  print_rectangle(9, 12, BOARD_M + 3, BOARD_M + HUD_WIDTH + 2);
  print_rectangle(13, 20, BOARD_M + 3, BOARD_M + HUD_WIDTH + 2);

  MVPRINTW(2, BOARD_M + 5, "SCORE");
  MVPRINTW(6, BOARD_M + 5, "HIGHSCORE");
  MVPRINTW(10, BOARD_M + 5, "LEVEL");
  MVPRINTW(14, BOARD_M + 5, "NEXT");
}

void print_msg(char *msg) {
  for (int i = 0; i < strlen(msg); i += BOARD_M - 1) {
    MVPRINTW(BOARD_N / 2 + i / (BOARD_M - 1), 1, "%*s", BOARD_M - 1, " ");
    MVPRINTW(
        BOARD_N / 2 + i / (BOARD_M - 1),
        strlen(msg + i) < BOARD_M ? (BOARD_M - strlen(msg + i)) / 2 + 1 : 1,
        "%.*s", BOARD_M - 1, msg + i);
  }
  refresh();
}

void print_intro(void) { print_msg(INTRO_MESSAGE); }
void print_exit(void) { print_msg(EXIT_MESSAGE); }
void print_gameover(void) { print_msg(GAMEOVER_MESSAGE); }

void print_rectangle(int top_y, int bottom_y, int left_x, int right_x) {
  MVADDCH(top_y, left_x, ACS_ULCORNER);

  int i = left_x + 1;

  for (; i < right_x; i++) MVADDCH(top_y, i, ACS_HLINE);
  MVADDCH(top_y, i, ACS_URCORNER);

  for (int i = top_y + 1; i < bottom_y; i++) {
    MVADDCH(i, left_x, ACS_VLINE);
    MVADDCH(i, right_x, ACS_VLINE);
  }

  MVADDCH(bottom_y, left_x, ACS_LLCORNER);
  i = left_x + 1;
  for (; i < right_x; i++) MVADDCH(bottom_y, i, ACS_HLINE);
  MVADDCH(bottom_y, i, ACS_LRCORNER);
}

void print_stats(GameInfo_t game_info) {
  MVPRINTW(3, BOARD_M + 8, "%7d", game_info.score);
  MVPRINTW(7, BOARD_M + 8, "%7d", game_info.high_score);
  MVPRINTW(11, BOARD_M + 8, "%7d", game_info.level);
  if (game_info.next)
    for (int i = 0; i < FIGURE_H; i++) {
      for (int j = 0; j < FIGURE_W; j++) {
        if (game_info.next[i][j] > 1) {
          MVADDCH(15 + i, BOARD_M + 7 + j * BOARD_SCALE - 1,
                  ACS_CKBOARD | COLOR_PAIR(game_info.next[i][j] & COLOR_MASK));
          MVADDCH(15 + i, BOARD_M + 7 + j * BOARD_SCALE,
                  ACS_CKBOARD | COLOR_PAIR(game_info.next[i][j] & COLOR_MASK));
        } else {
          MVADDCH(15 + i, BOARD_M + 7 + j * BOARD_SCALE - 1,
                  ' ' | COLOR_PAIR(0));
          MVADDCH(15 + i, BOARD_M + 7 + j * BOARD_SCALE, ' ' | COLOR_PAIR(0));
        }
      }
    }
  if (game_info.pause) {
    attrset(A_BLINK | COLOR_PAIR(1));
    MVPRINTW(21, BOARD_M + 4, "%-12s", "Pause");
    attrset(A_NORMAL);
  } else {
    MVPRINTW(21, BOARD_M + 4, "%-12s", "P for Pause");
  }
}

void print_board(GameInfo_t game_info) {
  BOARD_CLEAN;
  if (game_info.field)
    for (int i = ROWS_MAP - 1; i >= 0; i--) {
      for (int j = 0; j < COLS_MAP; j++) {
        if (game_info.field[i][j] > 0) {
          PRINT_OBJ(
              ROWS_MAP - i, j + MAP_PADDING,
              ACS_CKBOARD | COLOR_PAIR(game_info.field[i][j] & COLOR_MASK));
        } else {
          PRINT_CHAR(ROWS_MAP - i, j + MAP_PADDING, '.');
        }
      }
    }
  print_stats(game_info);
  refresh();
}

UserAction_t get_signal(int user_input) {
  UserAction_t ret = Up;
  for (UserAction_t i = Start; i <= Action; i++)
    if (KEYMAP[i] == user_input) ret = i;
  return ret;
}
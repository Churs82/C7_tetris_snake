#include "tetris.h"

int main(void) {
  WIN_INIT(30);
  setlocale(LC_ALL, "");
  print_overlay();
  game_loop();
  endwin();
  return SUCCESS;
}

void game_loop() {
  bool break_flag = 1;
  int signal = -1;
  while (break_flag) {
    if (getState() == START)
      print_intro();
    else if (getState() == GAME_OVER)
      print_gameover();
    else if (getState() == EXIT_STATE)
      print_exit();
    else
      print_board(updateCurrentState());
    signal = GET_USER_INPUT;
    if (signal > -1) {
      userInput(get_signal(signal), false);
    }
    break_flag = updateCurrentState().speed > 0;
  }
}
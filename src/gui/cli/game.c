#include "game.h"

int main(void) {
  WIN_INIT(30);
  setlocale(LC_ALL, "");
  print_overlay();
  game_loop();
  endwin();
  return SUCCESS;
}

/**
 * @brief The main game loop.
 *
 * Prints the game field and handles user input.
 *
 * The loop will break if the game is in the EXIT_STATE and the speed is 0.
 *
 * @return void
 */
void game_loop() {
  int signal = -1;
  GameInfo_t game_info = updateCurrentState();

  while (game_info.field) {
    print_board(game_info);
    /* User input */  
    signal = GET_USER_INPUT;
    if (signal > -1) {
      userInput(get_signal(signal), false);
    }
    game_info = updateCurrentState();
  }
}
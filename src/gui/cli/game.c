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
 * The loop will break if the game field is null, indicating the game has ended.
 *
 * @return void
 */
void game_loop() {
  intro();  

  GameInfo_t game_info = updateCurrentState();

  while (game_info.field) {
    
    print_board(game_info);
    
    /* User input */  
    int signal = GET_USER_INPUT;
    if (signal > -1) {
      if(get_signal(signal) == Terminate){
        end_game();
      } else {
        userInput(get_signal(signal), false);
      }
    }

    game_info = updateCurrentState();
  }
}

void intro(){
  print_intro();
  int signal = -1;
  while (get_signal(signal) != Start && get_signal(signal) != Terminate) {
    signal = GET_USER_INPUT;
  }
  userInput(get_signal(signal), false);
}

void end_game() {
  print_exit();
  int signal = -1;
  while (get_signal(signal) != Start && get_signal(signal) != Terminate) {
    signal = GET_USER_INPUT;
  }
  userInput(get_signal(signal), false);
}
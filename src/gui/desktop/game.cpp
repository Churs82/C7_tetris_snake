#include "controller.h"
#include "view.h"
#include "lib.h"
namespace s21 {
  /**
   * @brief The main function of the game.
   *
   * Initializes the game logic and GUI, and starts the game loop.
   *
   * @return int Exit status of the program.
   */

int main(void) {
    // Initialize the game logic and GUI
    std::unique_ptr<GameView> gameView = std::make_unique<GameView>();
    gameView->show();

    // WIN_INIT(30);
    // setlocale(LC_ALL, "");
    // print_overlay();
    // game_loop();
    // endwin();
    return SUCCESS;
  }
}  // namespace s21
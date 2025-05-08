#include "../../inc/lib.h"
#include "controller.h"
#include "view.h"

/**
 * @brief The main function of the game.
 *
 * Initializes the game logic and GUI, and starts the game loop.
 *
 * @return int Exit status of the program.
 */

int main(int argc, char** argv) {
  using namespace s21;
  QApplication app(argc, argv);

  // Initialize the game logic and GUI
  std::unique_ptr<GameView> gameView = std::make_unique<GameView>();
  gameView->show();

  // WIN_INIT(30);
  // setlocale(LC_ALL, "");
  // print_overlay();
  // game_loop();
  // endwin();
  return app.exec();
}

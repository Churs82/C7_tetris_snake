#include "game.h"

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
  std::unique_ptr<GameController> gameController =
      std::make_unique<GameController>();
  // app.installEventFilter(gameController.get());
  gameController->start();
  // WIN_INIT(30);
  // setlocale(LC_ALL, "");
  // print_overlay();
  // game_loop();
  // endwin();
  return app.exec();
}

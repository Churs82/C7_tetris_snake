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
  std::unique_ptr<GameController> game_controller =
      std::make_unique<GameController>();

  game_controller->StartGame();

  return app.exec();
}

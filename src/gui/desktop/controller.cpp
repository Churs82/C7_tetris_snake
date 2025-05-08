#include "controller.h"

// bool GameView::getUserInput(UserAction_t &action) {
//   bool input_received = false;
//   if (!_pendingInputQueue.isEmpty()) {
//     action = _pendingInputQueue.dequeue();
//     input_received = true;
//   }
//   return input_received;
// }

// void GameView::keyPressEvent(QKeyEvent *event) {
//   UserAction_t action;
//   bool actionProcessed = true;
//   switch (event->key()) {
//     case Qt::Key_Left:
//       action = Left;
//       break;
//     case Qt::Key_Right:
//       action = Right;
//       break;
//     case Qt::Key_Down:
//       action = Down;
//       break;
//     case Qt::Key_Space:
//       action = Pause;
//       break;
//     case Qt::Key_Return:
//     case Qt::Key_Enter:
//       action = Start;
//       break;
//     case Qt::Key_Escape:
//       action = Terminate;
//       break;
//     case Qt::Key_Up:
//       action = Up;
//       break;
//     case Qt::Key_Slash:
//       action = Action;
//       break;
//     default:
//       actionProcessed = false;
//       break;
//   }
//   if (actionProcessed) {
//     userInput(action, true);
//     update();
//   }
// }

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
}

void intro() {
}

void end_game() {

}
#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <QtGui/QKeyEvent>

#include "../../inc/lib.h"
#include "view.h"

namespace s21 {
class GameController {
  /**
   * @brief Handles keyboard input events.
   *
   * @param event The key event triggered by the user.
   */
  void keyPressEvent(QKeyEvent *event);
};
}  // namespace s21
#endif
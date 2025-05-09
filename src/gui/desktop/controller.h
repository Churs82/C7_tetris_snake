#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <QtCore/QQueue>
#include <QtGui/QKeyEvent>
#include <QtWidgets/QMainWindow>

#include "../../inc/lib.h"
#include "view.h"

namespace s21 {
class GameController : public QMainWindow {
  Q_OBJECT

 public:
  explicit GameController(QMainWindow *parent = nullptr);
  ~GameController() noexcept override;

 protected:
  void keyPressEvent(QKeyEvent *event) override;
  bool getUserInput(UserAction_t &action);

 private:
  QQueue<UserAction_t> _pendingInputQueue;
  std::unique_ptr<GameView> _gameView;
  std::unique_ptr<QMainWindow> _mainWindow;
};
}  // namespace s21
#endif
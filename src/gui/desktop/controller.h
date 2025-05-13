#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <QtCore/QQueue>
#include <QtCore/QTimer>
#include <QtGui/QKeyEvent>
#include <QtWidgets/QMainWindow>

#include "../../inc/lib.h"
#include "view.h"

namespace s21 {
class GameController : public QWidget {
  Q_OBJECT

 public:
  explicit GameController(QMainWindow *parent = nullptr);
  ~GameController() noexcept override;
  GameInfo_t getGameInfo();

 protected:
  bool eventFilter(QObject *object, QEvent *event) override;
  void keyPressEvent(QKeyEvent *event) override;
  bool getUserInput(UserAction_t &action);

 private:
  QQueue<UserAction_t> _pendingInputQueue;
  std::unique_ptr<GameView> _gameView;
  std::unique_ptr<QMainWindow> _mainWindow;
  std::unique_ptr<QTimer> _updateTimer;
};
}  // namespace s21
#endif
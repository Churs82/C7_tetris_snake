#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <QtCore/QQueue>
#include <QtCore/QTimer>
#include <QtGui/QKeyEvent>
#include <QtWidgets/QMainWindow>
#include <map>

#include "../../inc/lib.h"
#include "view.h"

namespace s21 {

class GameView;
class GameController : public QWidget {
  Q_OBJECT

 public:
  explicit GameController(QMainWindow *parent = nullptr);
  ~GameController() noexcept override;
  GameInfo_t getGameInfo();
  void start();

 protected:
  bool eventFilter(QObject *object, QEvent *event) override;
  void keyPressEvent(QKeyEvent *event) override;
  bool getUserInput(UserAction_t &action);

 private:
  QQueue<UserAction_t> _pendingInputQueue;
  std::unique_ptr<GameView> _gameView;
  std::unique_ptr<QMainWindow> _mainWindow;
  std::unique_ptr<QTimer> _updateTimer;

  /* Constants */
 private:
  const std::map<Qt::Key, UserAction_t> _key_map{
      {Qt::Key_Left, Left},    {Qt::Key_Right, Right},
      {Qt::Key_Up, Up},        {Qt::Key_Down, Down},
      {Qt::Key_Space, Action}, {Qt::Key_Return, Start},
      {Qt::Key_Enter, Start},  {Qt::Key_Escape, Terminate},
      {Qt::Key_P, Pause},      {Qt::Key_Space, Action}};
};
}  // namespace s21
#endif
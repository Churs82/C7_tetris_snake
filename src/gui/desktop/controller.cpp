#include "controller.h"
namespace s21 {

GameController::GameController(QMainWindow *parent) : QWidget(parent) {
  _gameView = std::make_unique<GameView>(parent);
  _updateTimer = std::make_unique<QTimer>(_gameView.get());
  connect(_updateTimer.get(), &QTimer::timeout, _gameView.get(),
          &GameView::render);
  _gameView->installEventFilter(this);
  _gameView->show();
  _gameView->setFocus();
  _updateTimer->start(30);
}
GameController::~GameController() noexcept {}

GameInfo_t GameController::getGameInfo() { return ::updateCurrentState(); }
bool GameController::eventFilter(QObject *object, QEvent *event) {
  if (object == _gameView.get()) {
    if (event->type() == QEvent::KeyPress) {
      QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
      keyPressEvent(keyEvent);
      return true;
    }
  }
  return QWidget::eventFilter(object, event);
}

void GameController::keyPressEvent(QKeyEvent *event) {
  UserAction_t action;
  bool actionProcessed = true;
  switch (event->key()) {
    case Qt::Key_Left:
      action = Left;
      break;
    case Qt::Key_Right:
      action = Right;
      break;
    case Qt::Key_Down:
      action = Down;
      break;
    case Qt::Key_Space:
      action = Action;
      break;
    case Qt::Key_Return:
    case Qt::Key_Enter:
      action = Start;
      break;
    case Qt::Key_Escape:
      action = Terminate;
      break;
    case Qt::Key_Up:
      action = Up;
      break;
    case Qt::Key_Slash:
      action = Action;
      break;
    case Qt::Key_P:
      action = Pause;
      break;
    default:
      actionProcessed = false;
      break;
  }
  if (actionProcessed) {
    ::userInput(action, true);
  }
  _gameView->update();
}

bool GameController::getUserInput(UserAction_t &action) {
  bool input_received = false;
  if (!_pendingInputQueue.isEmpty()) {
    action = _pendingInputQueue.dequeue();
    input_received = true;
  }
  return input_received;
}
}  // namespace s21

#include "controller.moc"

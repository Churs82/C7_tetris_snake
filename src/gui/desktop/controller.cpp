#include "controller.h"
namespace s21 {

GameController::GameController(QMainWindow *parent) : QWidget(parent) {
  _gameView = std::make_unique<GameView>(this);
}

void GameController::start() {
  _updateTimer = std::make_unique<QTimer>(_gameView.get());
  connect(_updateTimer.get(), &QTimer::timeout, _gameView.get(),
          &GameView::render);
  _gameView->installEventFilter(this);
  _gameView->setMessageModal(INTRO_MESSAGE);
  _gameView->show();
  _gameView->setFocus();
  _updateTimer->start(20);
}

GameController::~GameController() noexcept {}

GameInfo_t GameController::getGameInfo() {
  GameInfo_t gameInfo = ::updateCurrentState();
  if (gameInfo.field == nullptr) QApplication::quit();
  if ((!_gameView->getMessageModal()->isEmpty() && !gameInfo.pause) ||
      (_gameView->getMessageModal()->isEmpty() && gameInfo.pause)) {
    ::userInput(Pause, false);
    gameInfo = ::updateCurrentState();
  }
  return gameInfo;
}

bool GameController::eventFilter(QObject *object, QEvent *event) {
  if (object == _gameView.get()) {
    if (event->type() == QEvent::KeyPress) {
      QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
      keyPressEvent(keyEvent);
      return true;
    }
    if (event->type() == QEvent::Close) {
      QApplication::quit();
    }
  }
  return QWidget::eventFilter(object, event);
}

void GameController::keyPressEvent(QKeyEvent *event) {
  UserAction_t action = Start;
  bool actionProcessed = true;
  try {
    action = _key_map.at(static_cast<Qt::Key>(event->key()));
  } catch (const std::out_of_range &) {
    actionProcessed = false;
  }

  if (actionProcessed) {
    if (action != Pause) {
      _pendingInputQueue.enqueue(action);
    } else if (_gameView->getMessageModal()->isEmpty()) {
      _gameView->setMessageModal(PAUSE_MESSAGE);
    }
    if (_gameView->getMessageModal()->compare(EXIT_MESSAGE) != 0) {
      if (action == Terminate) {
        _gameView->setMessageModal(EXIT_MESSAGE);
      } else {
        ::userInput(action, false);
      }
    } else if (action == Start || action == Terminate) {
      _gameView->setMessageModal("");
      ::userInput(action, false);
    }
  }

  _gameView->update();
}

void GameController::processKeysQueue() {
  UserAction_t action;
  while (getUserInput(action)) {
    ::userInput(action, true);
  }
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

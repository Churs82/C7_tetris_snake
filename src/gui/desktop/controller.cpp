#include "controller.h"
namespace s21 {

/**
 * @brief Controller class for managing the game logic and user input.
 */
GameController::GameController(QMainWindow* parent) : QWidget(parent) {
  game_view_ = std::make_unique<GameView>(this);
}

/**
 * @brief Starts the game, initializes timers and GUI.
 */
void GameController::StartGame() {
  update_timer_ = std::make_unique<QTimer>(game_view_.get());
  connect(update_timer_.get(), &QTimer::timeout, game_view_.get(),
          &GameView::Render);
  game_view_->installEventFilter(this);
  game_view_->SetMessageModal(INTRO_MESSAGE);
  game_view_->show();
  game_view_->setFocus();
  update_timer_->start(20);
}

/**
 * @brief Destructor for GameController.
 */
GameController::~GameController() noexcept {}

/**
 * @brief Gets the current game info and manages pause/quit logic.
 * @return GameInfo_t The current game state information.
 */
GameInfo_t GameController::GetGameInfo() {
  ProcessKeysQueue();
  GameInfo_t game_info = ::updateCurrentState();
  if (game_info.field == nullptr) QApplication::quit();
  if (game_view_->GetMessageModal()->isEmpty() && game_info.pause) {
    game_view_->SetMessageModal(PAUSE_MESSAGE);
  }
  if (!game_info.pause &&
      !game_view_->GetMessageModal()->compare(PAUSE_MESSAGE)) {
    game_view_->ClearMessageModal();
  }
  return game_info;
}

/**
 * @brief Event filter for handling key and close events.
 * @param object The object receiving the event.
 * @param event The event to filter.
 * @return true if the event was handled, false otherwise.
 */
bool GameController::eventFilter(QObject* object, QEvent* event) {
  if (object == game_view_.get()) {
    if (event->type() == QEvent::KeyPress) {
      QKeyEvent* key_event = static_cast<QKeyEvent*>(event);
      keyPressEvent(key_event);
      return true;
    }
    if (event->type() == QEvent::Close) {
      QApplication::quit();
    }
  }
  return QWidget::eventFilter(object, event);
}

/**
 * @brief Handles key press events and enqueues user actions.
 * @param event The key event.
 */
void GameController::keyPressEvent(QKeyEvent* event) {
  UserAction_t action = Start;
  bool action_processed = true;
  try {
    action = key_map_.at(static_cast<Qt::Key>(event->key()));
  } catch (const std::out_of_range&) {
    action_processed = false;
  }

  if (action_processed) {
    keys_queue_.enqueue(action);
  }

  game_view_->update();
}

/**
 * @brief Processes all queued user actions.
 */
void GameController::ProcessKeysQueue() {
  UserAction_t action;
  while (GetUserInput(action)) {
    SendUserInput(action);
  }
}

/**
 * @brief Handles the close event and sends terminate input.
 * @param event The close event.
 */
void GameController::closeEvent(QCloseEvent* event) {
  userInput(Terminate, false);
  QWidget::closeEvent(event);
}

/**
 * @brief Sends a user action to the game logic, handling modal messages.
 * @param action The user action to send.
 */
void GameController::SendUserInput(UserAction_t action) {
  if (game_view_->GetMessageModal()->compare(EXIT_MESSAGE) &&
      game_view_->GetMessageModal()->compare(INTRO_MESSAGE)) {
    if (action == Terminate) {
      if (game_view_->GetMessageModal()->compare(PAUSE_MESSAGE))
        ::userInput(Pause, false);
      game_view_->SetMessageModal(EXIT_MESSAGE);
    } else {
      ::userInput(action, false);
    }
  } else if (action == Start || action == Terminate) {
    game_view_->ClearMessageModal();
    ::userInput(action, false);
  }
}

/**
 * @brief Retrieves the next user action from the queue.
 * @param action Reference to store the retrieved action.
 * @return true if an action was retrieved, false otherwise.
 */
bool GameController::GetUserInput(UserAction_t& action) {
  bool input_received = false;
  if (!keys_queue_.isEmpty()) {
    action = keys_queue_.dequeue();
    input_received = true;
  }
  return input_received;
}

}  // namespace s21

#include "controller.moc"

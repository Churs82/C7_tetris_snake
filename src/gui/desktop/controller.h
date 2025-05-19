#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <QtCore/QQueue>
#include <QtCore/QTimer>
#include <QtGui/QKeyEvent>
#include <QtWidgets/QMainWindow>
#include <map>

#include "lib.h"
#include "view.h"

namespace s21 {

class GameView;

/**
 * @brief Controller class for managing the game logic and user input.
 */
class GameController : public QWidget {
  Q_OBJECT

 public:
  /**
   * @brief Constructs a GameController with an optional parent.
   * @param parent Pointer to the QMainWindow parent.
   */
  explicit GameController(QMainWindow* parent = nullptr);

  /**
   * @brief Destructor for GameController.
   */
  ~GameController() noexcept override;

  /**
   * @brief Starts the game, initializes timers and GUI.
   */
  void StartGame();

  /**
   * @brief Gets the current game info and manages pause/quit logic.
   * @return GameInfo_t The current game state information.
   */
  GameInfo_t GetGameInfo();

  /**
   * @brief Event filter for handling key and close events.
   * @param object The object receiving the event.
   * @param event The event to filter.
   * @return true if the event was handled, false otherwise.
   */
  bool eventFilter(QObject* object, QEvent* event) override;

  /**
   * @brief Handles key press events and enqueues user actions.
   * @param event The key event.
   */
  void keyPressEvent(QKeyEvent* event) override;

  /**
   * @brief Processes all queued user actions.
   */
  void ProcessKeysQueue();

  /**
   * @brief Handles the close event and sends terminate input.
   * @param event The close event.
   */
  void closeEvent(QCloseEvent* event) override;

  /**
   * @brief Sends a user action to the game logic, handling modal messages.
   * @param action The user action to send.
   */
  void SendUserInput(UserAction_t action);

  /**
   * @brief Retrieves the next user action from the queue.
   * @param action Reference to store the retrieved action.
   * @return true if an action was retrieved, false otherwise.
   */
  bool GetUserInput(UserAction_t& action);

 private:
  QQueue<UserAction_t> keys_queue_;
  std::unique_ptr<GameView> game_view_;
  std::unique_ptr<QMainWindow> main_window_;
  std::unique_ptr<QTimer> update_timer_;

  /* Constants */
 private:
  const std::map<Qt::Key, UserAction_t> key_map_{
      {Qt::Key_Left, Left},    {Qt::Key_Right, Right},
      {Qt::Key_Up, Up},        {Qt::Key_Down, Down},
      {Qt::Key_Space, Action}, {Qt::Key_Return, Start},
      {Qt::Key_Enter, Start},  {Qt::Key_Escape, Terminate},
      {Qt::Key_P, Pause},      {Qt::Key_Space, Action}};
};

}  // namespace s21
#endif
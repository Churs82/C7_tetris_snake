#ifndef GAME_VIEW_QT_H
#define GAME_VIEW_QT_H

#define QT_FEATURE_MENU -1
#define COLOR_MASK 15
#define COLOR_MAP                                                        \
  std::array<QColor, 9> {                                                \
    Qt::color0, Qt::color1, Qt::darkBlue, Qt::darkGreen, Qt::darkYellow, \
        Qt::darkCyan, Qt::darkMagenta, Qt::darkRed, Qt::red              \
  }

#include <QtCore/QQueue>
#include <QtGui/QPainter>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMessageBox>
#include <QtWidgets/QStyle>

#include "../../inc/lib.h"
#include "controller.h"

namespace s21 {

class GameController;
/**
 * @class GameView
 * @brief Qt-based graphical interface.
 *
 * Manages game rendering using `QPainter` and processes user input via
 * `QKeyEvent`.
 */
class GameView : public QMainWindow {
  Q_OBJECT

 public:
  /**
   * @brief Constructs the Qt-based game view.
   *
   * @param logicBridge Pointer to `LogicGuiBridge` for interaction with the
   * game logic.
   * @param parent Pointer to the parent QWidget (optional).
   */
  explicit GameView(GameController *parent = nullptr);
  virtual ~GameView() noexcept;

  void setMessageModal(const char *message = nullptr);

  QString *getMessageModal();

  void clearMessageModal();

 protected:
  /**
   * @brief Handles game rendering events.
   *
   * @param event The paint event triggered for rendering.
   */
  void paintEvent(QPaintEvent * /*event*/) override;

 private:
  static constexpr int _screen_unit =
      20;  ///< Padding size, one game element block.
  static constexpr int _game_field_width =
      COLS_MAP * _screen_unit;  ///< Width of the game field in pixels.
  static constexpr int _game_field_height =
      ROWS_MAP * _screen_unit;  ///< Height of the game field in pixels.

  static constexpr int _box_dimension =
      FIGURE_H * _screen_unit;  ///< Modal & next piece preview box in tetris.
  static constexpr int _app_width =
      _game_field_width + _box_dimension + 6 * _screen_unit;
  static constexpr int _app_height = _game_field_height + 2 * _screen_unit;

  GameController *_controller;              ///< Pointer to the controller.
  std::unique_ptr<QString> _message_modal;  ///< Pointer to the message modal.

  /**
   * @brief Converts a game color index to an RGB color.
   *
   * Used to determine the visual representation of game elements.
   *
   * @param color The numerical index of the game color (1-7).
   * @return `QColor` representing the corresponding RGB color.
   */
  QColor colorToRGB(long unsigned int);

  /**
   * @brief Renders the game state on the Qt window.
   *
   * Handles the drawing of the field, the current piece, the next piece
   * preview, and game status messages like "Game Over" or "Press ENTER to
   * Start".
   *
   * @param painter Reference to `QPainter` used for rendering.
   * @param gameInfo Pointer to `GameInfo_t` containing game statistics.
   * @param dynamicElement Pointer to a 2D array representing dynamic game
   * elements.
   */
  void renderGame(QPainter &painter, const ::GameInfo_t &gameInfo);

  /**
   * @brief Draws the game field and dynamic elements.
   *
   * The function first draws the background grid, then overlays any moving
   * elements.
   *
   * @param painter Reference to `QPainter` used for rendering.
   * @param gameInfo Pointer to `GameInfo_t` containing the current game field
   * data.
   */
  void drawField(QPainter &painter, int **const field);

  /**
   * @brief Draws the "next" preview box.
   *
   * If a next piece is available, this function outlines a box and renders the
   * piece.
   *
   * @param painter Reference to `QPainter` used for rendering.
   * @param gameInfo Pointer to `GameInfo_t` containing the next piece data.
   */
  void drawNext(QPainter &painter, int **const next);

 private:
  /**
   * @brief Displays a modal message over the game screen.
   *
   * Used to show messages like "Press ENTER to Start".
   *
   * @param painter Reference to `QPainter` used for rendering.
   * @param msg The message text to display in the modal.
   */
  void showModal(QPainter &painter, const QString &msg);

  /**
   * @brief Renders a text label on the game screen.
   *
   * Used for displaying labels such as "Score: 100" or "Level: 3".
   *
   * @param painter Reference to `QPainter` used for rendering.
   * @param label The text label to display.
   * @param value The numerical value to display next to the label (-1 if no
   * value).
   * @param position The screen position where the label should be drawn.
   */
  void renderLabel(QPainter &painter, const QString &label, int value,
                   QPoint position);

  /**
   * @brief Draws a border around a given rectangular area.
   *
   * Used for outlining game areas, such as the game field or next tetromino
   * preview box.
   *
   * @param painter Reference to `QPainter` used for rendering.
   * @param rect The `QRect` defining the area to be outlined.
   */
  void drawBorder(QPainter &painter, const QRect &rect);

 public slots:
  /**
   * @brief Renders the game screen when the timer triggers a refresh.
   */
  void render();
};

}  // namespace s21

#endif  // GAME_VIEW_QT_H

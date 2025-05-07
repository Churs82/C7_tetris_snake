#ifndef GAME_VIEW_QT_H
#define GAME_VIEW_QT_H

#include <QtWidgets/QApplication>
#include <QtGui/QKeyEvent>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMessageBox>
#include <QtGui/QPainter>
#include <QtCore/QQueue>

#include "lib.h"

namespace s21 {

/**
 * @class GameViewQt
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
  explicit GameView(QWidget *parent = nullptr);


 protected:
  /**
   * @brief Handles game rendering events.
   *
   * @param event The paint event triggered for rendering.
   */
  void paintEvent(QPaintEvent *event) override;

  /**
   * @brief Handles keyboard input events.
   *
   * @param event The key event triggered by the user.
   */
  void keyPressEvent(QKeyEvent *event) override;

 private:

  static constexpr int _game_field_width =
      200;  ///< Width of the game field in pixels.
  static constexpr int _game_field_height =
      400;  ///< Height of the game field in pixels.

  static constexpr int _screen_unit =
      20;  ///< Padding size, one game element block.
  static constexpr int _box_dimension =
      100;  ///< Modal & next piece preview box in tetris.
  static constexpr int _app_width =
      _game_field_width + _box_dimension + 6 * _screen_unit;
  static constexpr int _app_height = _game_field_height + 2 * _screen_unit;

  /**
   * @brief Converts a game color index to an RGB color.
   *
   * Used to determine the visual representation of game elements.
   *
   * @param color The numerical index of the game color (1-7).
   * @return `QColor` representing the corresponding RGB color.
   */
  QColor colorToRGB(int color);

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
  void renderGame(QPainter &painter, const ::GameInfo_t *gameInfo, int **dynamicElement);

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
  void drawField(QPainter &painter, const GameInfo_t *gameInfo);

  /**
   * @brief Draws the "next" preview box.
   *
   * If a next piece is available, this function outlines a box and renders the
   * piece.
   *
   * @param painter Reference to `QPainter` used for rendering.
   * @param gameInfo Pointer to `GameInfo_t` containing the next piece data.
   */
  void drawNextPiece(QPainter &painter, const GameInfo_t *gameInfo);

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

 private slots:
  /**
   * @brief Renders the game screen when the timer triggers a refresh.
   */
  void render();
};

}  // namespace s21

#endif  // GAME_VIEW_QT_H

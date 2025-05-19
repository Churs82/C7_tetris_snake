#ifndef GAME_VIEW_QT_H
#define GAME_VIEW_QT_H

#define QT_FEATURE_MENU -1
#define COLOR_MASK 15

#include <QtCore/QQueue>
#include <QtGui/QPainter>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMessageBox>
#include <QtWidgets/QStyle>

#include "controller.h"
#include "lib.h"

namespace s21 {

class GameController;

class GameView : public QMainWindow {
  Q_OBJECT
 public:
  /**
   * @brief Constructs a GameView with an optional parent controller.
   * @param parent Pointer to the GameController parent.
   */
  explicit GameView(GameController* parent = nullptr);

  /**
   * @brief Destructor for GameView.
   */
  ~GameView() noexcept override;

  /**
   * @brief Renders the game view.
   */
  void Render();

  /**
   * @brief Handles paint events for the game view.
   * @param event The paint event.
   */
  void paintEvent(QPaintEvent*) override;

  /**
   * @brief Renders the game field and elements.
   * @param painter The QPainter to draw on.
   * @param game_info The current game state information.
   */
  void RenderGame(QPainter& painter, const ::GameInfo_t& game_info);

  /**
   * @brief Draws the game field.
   * @param painter The QPainter to draw on.
   * @param field The field array.
   */
  void DrawField(QPainter& painter, int** const field);

  /**
   * @brief Draws the border around the game field.
   * @param painter The QPainter to draw on.
   * @param rect The rectangle area for the border.
   */
  void DrawBorder(QPainter& painter, const QRect& rect);

  /**
   * @brief Draws the next figure preview.
   * @param painter The QPainter to draw on.
   * @param next The next figure array.
   */
  void DrawNext(QPainter& painter, int** const next);

  /**
   * @brief Converts a color value to QColor.
   * @param color The color value.
   * @return QColor representation.
   */
  QColor ColorToRgb(unsigned long color);

  /**
   * @brief Shows a modal message on the game view.
   * @param painter The QPainter to draw on.
   * @param msg The message to display.
   */
  void ShowModal(QPainter& painter, const QString& msg);

  /**
   * @brief Draws a label with the given text and number at the given position.
   * @param painter The QPainter to draw on.
   * @param label The label text.
   * @param number The number to append to the label (if >= 0).
   * @param position The position to draw the label at.
   */
  void RenderLabel(QPainter& painter, const QString& label, int number,
                   QPoint position);

  /**
   * @brief Sets the modal message to display.
   * @param message The message string.
   */
  void SetMessageModal(const char* message);

  /**
   * @brief Gets the current modal message.
   * @return Pointer to the message string.
   */
  QString* GetMessageModal();

  /**
   * @brief Clears the current modal message.
   */
  void ClearMessageModal();

 private:
  static constexpr int color_mask_ = COLOR_MASK;
  const std::array<QColor, 9> color_map_{
      Qt::color0,      Qt::color1,     Qt::darkBlue,
      Qt::darkGreen,   Qt::darkYellow, Qt::darkCyan,
      Qt::darkMagenta, Qt::darkRed,    Qt::red};
  static constexpr int rows_map_ = ROWS_MAP;
  static constexpr int cols_map_ = COLS_MAP;
  static constexpr int figure_h_ = FIGURE_H;
  static constexpr int figure_w_ = FIGURE_W;
  static constexpr int screen_unit_ = 20;
  static constexpr int game_field_width_ = cols_map_ * screen_unit_;
  static constexpr int game_field_height_ = rows_map_ * screen_unit_;
  static constexpr int box_dimension_ = figure_h_ * screen_unit_;
  static constexpr int app_width_ =
      game_field_width_ + box_dimension_ + 6 * screen_unit_;
  static constexpr int app_height_ = game_field_height_ + 2 * screen_unit_;

  GameController* controller_;
  std::unique_ptr<QString> message_modal_;
};

}  // namespace s21

#endif  // GAME_VIEW_QT_H

#include "view.h"

namespace s21 {

GameView::GameView(QMainWindow *parent) : QMainWindow(parent) {
  setFixedSize(_app_width, _app_height);
}

GameView::~GameView() noexcept {};

void GameView::render() { update(); }

void GameView::paintEvent(QPaintEvent * /*event*/) {
  QPainter painter(this);
  renderGame(painter, ::updateCurrentState());
}

void GameView::renderGame(QPainter &painter, const ::GameInfo_t &gameInfo) {
  drawField(painter, gameInfo.field);
  drawBorder(painter, QRect(_screen_unit, _screen_unit, COLS_MAP * _screen_unit,
                            ROWS_MAP * _screen_unit));
  drawNext(painter, gameInfo.next);
  renderLabel(painter, "Score: ", gameInfo.score,
              QPoint(_game_field_width + 2 * _screen_unit, _screen_unit + 10));
  renderLabel(
      painter, "High Score: ", gameInfo.high_score,
      QPoint(_game_field_width + 2 * _screen_unit, _screen_unit * 2 + 10));
  renderLabel(
      painter, "Level: ", gameInfo.level,
      QPoint(_game_field_width + 2 * _screen_unit, _screen_unit * 3 + 10));

  if (gameInfo.pause) showModal(painter, "Press any key to Resume");
}

void GameView::drawField(QPainter &painter, int **const field) {
  for (int i = 0; i < ROWS_MAP; ++i) {
    for (int j = 0; j < COLS_MAP; ++j) {
      int color = field[i][j] & COLOR_MASK;
      if (color) {
        painter.setBrush(colorToRGB(color));
        painter.drawRect((j + 1) * _screen_unit, (ROWS_MAP - i) * _screen_unit,
                         _screen_unit, _screen_unit);
      }
    }
  }
}

void GameView::drawBorder(QPainter &painter, const QRect &rect) {
  painter.setPen(QPen(Qt::black, 2));
  painter.setBrush(Qt::NoBrush);
  painter.drawRect(rect);
}

void GameView::drawNext(QPainter &painter, int **const next) {
  if (next) {
    constexpr int box_x = _game_field_width + 2 * _screen_unit;
    constexpr int box_y = _game_field_height - _box_dimension + _screen_unit;
    constexpr int box_width = _box_dimension;
    constexpr int box_height = _box_dimension;

    drawBorder(painter, QRect(box_x, box_y, box_width, box_height));
    renderLabel(painter, "Next:", -1, QPoint(box_x, box_y - 10));

    for (int i = 0; i < FIGURE_H; i++) {
      for (int j = 0; j < FIGURE_W; j++) {
        int color = next[i][j] & COLOR_MASK;
        if (color) {
          painter.setBrush(colorToRGB(color));
          painter.drawRect(j * _screen_unit, i * _screen_unit, _screen_unit,
                           _screen_unit);
        }
      }
    }
  }
}

QColor GameView::colorToRGB(long unsigned int color) {
  if (color < 1 || color >= COLOR_MAP.size()) color = 0;
  return COLOR_MAP[color];
}

void GameView::showModal(QPainter &painter, const QString &msg) {
  QRect modalRect(_screen_unit * 3, _app_height / 2 - _box_dimension / 4,
                  _app_width - 6 * _screen_unit, _box_dimension / 2);
  painter.setBrush(Qt::white);
  painter.drawRect(modalRect);
  drawBorder(painter, modalRect);

  painter.setPen(Qt::black);
  QFont font("Noto Mono", 13, QFont::Bold);
  painter.setFont(font);
  painter.drawText(modalRect, Qt::AlignCenter, msg);
}

/**
 * @brief Draws a label with the given text and number (if any) at the given
 * position.
 * @param painter The QPainter to draw on.
 * @param label The label text.
 * @param number The number to append to the label (if >= 0).
 * @param position The position to draw the label at.
 */
void GameView::renderLabel(QPainter &painter, const QString &label, int number,
                           QPoint position) {
  painter.setPen(Qt::black);
  QFont font("Noto Mono", 12, QFont::Bold);
  painter.setFont(font);

  QString text = label;
  if (number != -1) text += QString::number(number);

  painter.drawText(position, text);
}

}  // namespace s21
#include "view.moc"
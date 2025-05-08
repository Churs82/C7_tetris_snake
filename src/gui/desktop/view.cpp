#include "view.h"

namespace s21 {

GameView::GameView(QWidget *parent) : QMainWindow(parent) {
  setFixedSize(_app_width, _app_height);
  show();
}

void GameView::render() { update(); }

void GameView::paintEvent(QPaintEvent * /*event*/) {
  QPainter painter(this);

  renderGame(painter, ::updateCurrentState());
}

void GameView::renderGame(QPainter &painter, GameInfo_t gameInfo) {
  drawField(painter, &gameInfo);
  drawBorder(painter, QRect(_screen_unit, _screen_unit, COLS_MAP * _screen_unit,
                            ROWS_MAP * _screen_unit));
  drawNextPiece(painter, &gameInfo);
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

void GameView::drawField(QPainter &painter, GameInfo_t *gameInfo) {
  for (int i = 0; i < ROWS_MAP; i++) {
    for (int j = 0; j < COLS_MAP; j++) {
      int color = gameInfo->field[i][j];
      if (color) {
        painter.setBrush(colorToRGB(color));
        painter.drawRect(j * _screen_unit, i * _screen_unit, _screen_unit,
                         _screen_unit);
      }
    }
  }
}

void GameView::drawBorder(QPainter &painter, const QRect &rect) {
  painter.setPen(QPen(Qt::black, 2));
  painter.setBrush(Qt::NoBrush);
  painter.drawRect(rect);
}

void GameView::drawNextPiece(QPainter &painter, const GameInfo_t *gameInfo) {
  if (gameInfo->next) {
    constexpr int box_x = _game_field_width + 2 * _screen_unit;
    constexpr int box_y = _game_field_height - _box_dimension + _screen_unit;
    constexpr int box_width = _box_dimension;
    constexpr int box_height = _box_dimension;

    drawBorder(painter, QRect(box_x, box_y, box_width, box_height));
    renderLabel(painter, "Next:", -1, QPoint(box_x, box_y - 10));

    for (int i = 0; i < FIGURE_H; i++) {
      for (int j = 0; j < FIGURE_W; j++) {
        int color = gameInfo->next[i][j];
        if (color) {
          painter.setBrush(colorToRGB(color));
          painter.drawRect(j * _screen_unit, i * _screen_unit, _screen_unit,
                           _screen_unit);
        }
      }
    }
  }
}

QColor GameView::colorToRGB(int color) {
  switch (color) {
    case 1:
      return QColor(92, 84, 164, 255);  // I
    case 2:
      return QColor(252, 164, 124, 255);  // J
    case 3:
      return QColor(204, 100, 148, 255);  // L
    case 4:
      return QColor(228, 124, 140, 255);  // O
    case 5:
      return QColor(164, 92, 164, 255);  // S
    case 6:
      return QColor(252, 196, 132, 255);  // T
    case 7:
      return QColor(140, 108, 172, 255);  // Z
    default:
      return QColor(Qt::black);
  }
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

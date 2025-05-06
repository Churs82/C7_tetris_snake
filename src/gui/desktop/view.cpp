#include "view.h"

namespace s21 {

GameViewQt::GameViewQt(LogicGuiBridge *logic_bridge, QWidget *parent)
    : QMainWindow(parent), _logic_bridge(logic_bridge) {
  setFixedSize(_app_width, _app_height);
  show();
}

void GameViewQt::render() { update(); }

bool GameViewQt::getUserInput(UserAction_t &action) {
  bool input_received = false;
  if (!_pendingInputQueue.isEmpty()) {
    action = _pendingInputQueue.dequeue();
    input_received = true;
  }
  return input_received;
}

void GameViewQt::keyPressEvent(QKeyEvent *event) {
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
      action = Pause;
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
    default:
      actionProcessed = false;
      break;
  }
  if (actionProcessed) {
    userInput(action, true);
    update();
  }
}

void GameViewQt::paintEvent(QPaintEvent * /*event*/) {
  QPainter painter(this);

  renderGame(painter, _logic_bridge->getCurrentState(),
             _logic_bridge->getGameInfo(), _logic_bridge->getDynamicElement());
}

void GameViewQt::renderGame(QPainter &painter, GameState_t state,
                            const GameInfo_t *gameInfo, int **dynamicElement) {
  if (state == STATE_START)
    showModal(painter, "Press ENTER to Start");
  else if (state == STATE_GAME_OVER)
    showModal(painter, "GAME OVER!");
  else {
    drawField(painter, gameInfo, dynamicElement);
    drawBorder(painter, QRect(_screen_unit, _screen_unit, WIDTH * _screen_unit,
                              HEIGHT * _screen_unit));
    drawNextPiece(painter, gameInfo);
    renderLabel(
        painter, "Score: ", gameInfo->score,
        QPoint(_game_field_width + 2 * _screen_unit, _screen_unit + 10));
    renderLabel(
        painter, "High Score: ", gameInfo->high_score,
        QPoint(_game_field_width + 2 * _screen_unit, _screen_unit * 2 + 10));
    renderLabel(
        painter, "Level: ", gameInfo->level,
        QPoint(_game_field_width + 2 * _screen_unit, _screen_unit * 3 + 10));

    if (gameInfo->pause) showModal(painter, "Press SPACE to Resume");
  }
}

void GameViewQt::drawElement(QPainter &painter, GameElement_t *element) {
  for (int i = 0; i < element->rows; i++) {
    for (int j = 0; j < element->cols; j++) {
      int color = element->matrix[i][j];
      if (color) {
        painter.setBrush(colorToRGB(color));
        painter.drawRect((element->x_offset + j) * _screen_unit,
                         (element->y_offset + i) * _screen_unit, _screen_unit,
                         _screen_unit);
      }
    }
  }
}

void GameViewQt::drawField(QPainter &painter, const GameInfo_t *gameInfo,
                           int **dynamicElement) {
  GameElement_t game_field = {gameInfo->field, HEIGHT, WIDTH, 1, 1};
  drawElement(painter, &game_field);
  GameElement_t snake = {dynamicElement, HEIGHT, WIDTH, 1, 1};
  drawElement(painter, &snake);
}

void GameViewQt::drawBorder(QPainter &painter, const QRect &rect) {
  painter.setPen(QPen(Qt::black, 2));
  painter.setBrush(Qt::NoBrush);
  painter.drawRect(rect);
}

void GameViewQt::drawNextPiece(QPainter &painter, const GameInfo_t *gameInfo) {
  if (gameInfo->next) {
    constexpr int box_x = _game_field_width + 2 * _screen_unit;
    constexpr int box_y = _game_field_height - _box_dimension + _screen_unit;
    constexpr int box_width = _box_dimension;
    constexpr int box_height = _box_dimension;

    drawBorder(painter, QRect(box_x, box_y, box_width, box_height));
    renderLabel(painter, "Next:", -1, QPoint(box_x, box_y - 10));
    GameElement_t next_piece = {gameInfo->next, 4, 4, box_x / _screen_unit + 1,
                                box_y / _screen_unit + 2};
    drawElement(painter, &next_piece);
  }
}

QColor GameViewQt::colorToRGB(int color) {
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

void GameViewQt::showModal(QPainter &painter, const QString &msg) {
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

void GameViewQt::renderLabel(QPainter &painter, const QString &label,
                             int number, QPoint position) {
  painter.setPen(Qt::black);
  QFont font("Noto Mono", 12, QFont::Bold);
  painter.setFont(font);

  QString text = label;
  if (number != -1) text += QString::number(number);

  painter.drawText(position, text);
}

}  // namespace s21

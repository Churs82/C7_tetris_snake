#include "view.h"

namespace s21 {

GameView::GameView(GameController* parent) : QMainWindow(parent) {
  setFixedSize(app_width_, app_height_);
#ifdef G_TITLE
  setWindowTitle(G_TITLE);
#endif
  setPalette(QApplication::style()->standardPalette());
  setAutoFillBackground(true);
  setStyleSheet("");
  controller_ = parent;
}

GameView::~GameView() noexcept {}

void GameView::Render() { update(); }

void GameView::paintEvent(QPaintEvent* /*event*/) {
  QPainter painter(this);
  RenderGame(painter, controller_->GetGameInfo());
}

void GameView::RenderGame(QPainter& painter, const ::GameInfo_t& game_info) {
  if (game_info.field != nullptr) DrawField(painter, game_info.field);
  DrawBorder(painter,
             QRect(screen_unit_, screen_unit_, cols_map_ * screen_unit_,
                   rows_map_ * screen_unit_));
  if (game_info.next != nullptr) DrawNext(painter, game_info.next);
  RenderLabel(painter, "Score: ", game_info.score,
              QPoint(game_field_width_ + 2 * screen_unit_, screen_unit_ + 10));
  RenderLabel(
      painter, "High Score: ", game_info.high_score,
      QPoint(game_field_width_ + 2 * screen_unit_, screen_unit_ * 2 + 10));
  RenderLabel(
      painter, "Level: ", game_info.level,
      QPoint(game_field_width_ + 2 * screen_unit_, screen_unit_ * 3 + 10));
  if (game_info.pause) {
    RenderLabel(
        painter, "Paused", -1,
        QPoint(game_field_width_ + 2 * screen_unit_, game_field_height_));
  } else {
    RenderLabel(
        painter, "P for pause", -1,
        QPoint(game_field_width_ + 2 * screen_unit_, game_field_height_));
  }
  if (!GetMessageModal()->isEmpty()) ShowModal(painter, *GetMessageModal());
}

void GameView::DrawField(QPainter& painter, int** const field) {
  for (int i = 0; i < rows_map_; ++i) {
    for (int j = 0; j < cols_map_; ++j) {
      int color = field[i][j] & color_mask_;
      if (color) {
        painter.setBrush(ColorToRgb(color));
        painter.drawRect((j + 1) * screen_unit_, (rows_map_ - i) * screen_unit_,
                         screen_unit_, screen_unit_);
      }
    }
  }
}

void GameView::DrawBorder(QPainter& painter, const QRect& rect) {
  painter.setPen(QPen(Qt::black, 2));
  painter.setBrush(Qt::NoBrush);
  painter.drawRect(rect);
}

void GameView::DrawNext(QPainter& painter, int** const next) {
  constexpr int box_x = game_field_width_ + 2 * screen_unit_;
  constexpr int box_y = game_field_height_ - box_dimension_ - 2 * screen_unit_;
  constexpr int box_width = box_dimension_;
  constexpr int box_height = box_dimension_;

  DrawBorder(painter, QRect(box_x, box_y, box_width, box_height));
  RenderLabel(painter, "Next:", -1, QPoint(box_x, box_y - 10));

  if (next) {
    for (int i = 0; i < figure_h_; i++) {
      for (int j = 0; j < figure_w_; j++) {
        int color = next[i][j] & color_mask_;
        if (color) {
          painter.setBrush(ColorToRgb(color));
          painter.drawRect(j * screen_unit_ + box_x, box_y + i * screen_unit_,
                           screen_unit_, screen_unit_);
        }
      }
    }
  }
}

QColor GameView::ColorToRgb(unsigned long color) {
  if (color < 1 || color >= color_map_.size()) color = 0;
  return color_map_[color];
}

void GameView::ShowModal(QPainter& painter, const QString& msg) {
  QRect modal_rect(screen_unit_ * 3, app_height_ / 2 - box_dimension_,
                   app_width_ - 6 * screen_unit_, box_dimension_);
  painter.setBrush(Qt::white);
  painter.drawRect(modal_rect);
  DrawBorder(painter, modal_rect);

  painter.setPen(Qt::black);
  QFont font("Noto Mono", 13, QFont::Bold);
  painter.setFont(font);
  painter.drawText(modal_rect, Qt::AlignCenter | Qt::TextWordWrap, msg);
}

/**
 * @brief Draws a label with the given text and number (if any) at the given
 * position.
 * @param painter The QPainter to draw on.
 * @param label The label text.
 * @param number The number to append to the label (if >= 0).
 * @param position The position to draw the label at.
 */
void GameView::RenderLabel(QPainter& painter, const QString& label, int number,
                           QPoint position) {
  // painter.setPen(Qt::color0);
  // painter.setBrush(Qt::NoBrush);
  QFont font("Noto Mono", 12, QFont::Bold);
  painter.setFont(font);

  QString text = label;
  if (number != -1) text += QString::number(number);

  painter.drawText(position, text);
}

void GameView::SetMessageModal(const char* message) {
  message_modal_.reset(new QString(QString::fromUtf8(message)));
}

QString* GameView::GetMessageModal() { return message_modal_.get(); }

void GameView::ClearMessageModal() { message_modal_.reset(new QString); }

}  // namespace s21
#include "view.moc"
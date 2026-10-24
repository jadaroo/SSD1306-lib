#include "figure/rect.hpp"
#include "display/display.hpp"
#include "figure/figure.hpp"
#include <cstdint>

SSD1306::Rect::Rect(Point pos, uint8_t height, uint8_t width) : Figure(pos), height_(height), width_(width) {}

void SSD1306::Rect::draw(DISPLAY &display) {
  if(width_ == 0) return;

  for(int16_t y = pos().second; y < pos().second + height_; y++) {
    display.fill_hor_line(pos().first, pos().first + width_ - 1, y);
  }
}

void SSD1306::Rect::clear_figure(DISPLAY &display) {
  if(width_ == 0) return;

  for(int16_t y = pos().second; y < pos().second + height_; y++) {
    display.clear_hor_line(pos().first, pos().first + width_ - 1, y);
  }
}

void SSD1306::Rect::move_to(Point new_pos, DISPLAY &display) {
  clear_figure(display);
  set_pos(new_pos);
  draw(display);
}

void SSD1306::Rect::move_by(Point shift, DISPLAY &display) {
  Point new_pos = {pos().first + shift.first, pos().second + shift.second};
  move_to(new_pos, display);
}

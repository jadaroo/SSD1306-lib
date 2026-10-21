#include "figure/rect.hpp"
#include "display/display.hpp"
#include "figure/figure.hpp"
#include <cstdint>

SSD1306::Rect::Rect(Point pos, uint8_t height, uint8_t width) : Figure(pos), height_(height), width_(width){}

void SSD1306::Rect::draw(DISPLAY &display){
  for(uint8_t i = pos().second; i < (pos().second + height_); i++){
    for(uint8_t j = pos().first; j < (pos().first + width_); j++){
     display.set_pixel({j, i});
    }
  }
}

void SSD1306::Rect::delete_figure(DISPLAY &display){
  for(uint8_t i = pos().second; i < (pos().second + height_); i++){
    for(uint8_t j = pos().first; j < (pos().first + width_); j++){
      display.clear_pixel({j, i});
    }
  }
}

void SSD1306::Rect::move_to(Point new_pos, DISPLAY &display){
  delete_figure(display);
  set_pos(new_pos);
  draw(display);
}

void SSD1306::Rect::move_by(Point shift, DISPLAY &display){
  Point new_pos = {pos().first + shift.first, pos().second + shift.second};
  move_to(new_pos, display);
}
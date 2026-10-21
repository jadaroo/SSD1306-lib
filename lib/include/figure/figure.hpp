#pragma once

#include "display/display.hpp"

namespace SSD1306 {

class Figure
{
public:
  Figure(Point pos) : pos_(pos) {};
  virtual void draw(DISPLAY &display) = 0;
  virtual void move_to(Point new_pos, DISPLAY &display) = 0;
  virtual void move_by(Point shift, DISPLAY &display) = 0;
  virtual void delete_figure(DISPLAY &display) = 0;
  Point pos() { return pos_; }
  void set_pos(Point new_pos) { pos_ = new_pos; }
  virtual ~Figure() = default;

private:
  Point pos_;
};

}; // namespace SSD1306

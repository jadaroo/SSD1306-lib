#pragma once

#include "display/display.hpp"
#include "figure/figure.hpp"
#include <cstdint>
#include <pico/types.h>

namespace SSD1306 {
class Circle : public Figure
{
public:
  Circle(Point center_pos, uint8_t rad);
  void draw(DISPLAY &display) override;
  void move_to(Point new_pos, DISPLAY &display) override;
  void move_by(Point shift, DISPLAY &display) override;
  void clear_figure(DISPLAY &display) override;

private:
  uint8_t rad_;
};
}; // namespace SSD1306

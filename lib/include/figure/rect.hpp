#pragma once

#include "display/display.hpp"
#include "figure/figure.hpp"
#include <cstdint>
#include <pico/types.h>

namespace SSD1306 {
  class Rect : public Figure{
    public:
      Rect(Point pos, uint8_t height, uint8_t width);
      void draw(DISPLAY &display) override;
      void move_to(Point new_pos, DISPLAY &display) override;
      void move_by(Point shift, DISPLAY &display) override;
      void delete_figure(DISPLAY &display) override;
    private:
      uint8_t height_;
      uint8_t width_;
  };
};
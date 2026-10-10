#pragma once

#include "display/display.hpp"
#include "figure/figure.hpp"
#include <cstdint>
#include <pico/types.h>
#include <utility>

namespace SSD1306 {
  class Circle : public Figure{
    public:
      Circle(std::pair<uint8_t, uint8_t> center_pos, uint8_t rad);
      void draw(DISPLAY &display) override;
      void move_to(std::pair<uint8_t, uint8_t> new_pos, DISPLAY &display) override;
      void move_by(std::pair<uint8_t, uint8_t> shift, DISPLAY &display) override;
      void delete_figure(DISPLAY &display) override;
    private:
      uint8_t rad_;
  };
};
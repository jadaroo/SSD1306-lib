#pragma once

#include <cstdint>
#include <pico/types.h>
#include <utility>

namespace SSD1306 {
  struct Rect{
    Rect(uint8_t width, uint8_t height, std::pair<uint8_t, uint8_t> pos) : width_(width), height_(height), pos_(pos){}
  
    uint8_t width_;
    uint8_t height_;
    std::pair<uint8_t, uint8_t> pos_;
  };
};
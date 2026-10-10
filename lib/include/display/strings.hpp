#pragma once

#include <cstdint>
#include <string_view>
#include "point.hpp"
#include <utility>

namespace SSD1306{
  struct String {
    String(std::string_view string, Point point) : string_(string), point_(point){}
  
    std::string_view string_;
    Point point_ = {0, 0};
  };
};
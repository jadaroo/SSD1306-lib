#pragma once

#include "point.hpp"
#include <string_view>

namespace SSD1306 {
struct String
{
  String(std::string_view string, Point point) : string_(string), point_(point) {}

  std::string_view string_;
  Point point_ = {0, 0};
};
}; // namespace SSD1306

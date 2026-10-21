#pragma once

#include "point.hpp"
#include <cstdio>
#include <cstring>

constexpr uint8_t max_string_len = 168;

namespace SSD1306 {

struct String
{
  String(Point point, const char *string, uint8_t len) : point_(point) {
    if(len > max_string_len) {
      printf("Too long string\n");
      return;
    }
    memcpy(string_, string, len);
    len_ = len;
  }

  char string_[max_string_len]{0};
  uint8_t len_ = 0;
  Point point_;
};
}; // namespace SSD1306

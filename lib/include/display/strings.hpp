#pragma once

#include <cstdint>
#include <string_view>
#include <utility>

namespace SSD1306{
  struct String {
    String(std::string_view string, std::pair<uint8_t, uint8_t> point) : string_(string), point_(point){}
  
    std::string_view string_;
    std::pair<uint8_t, uint8_t> point_ = {0, 0};
  };
};
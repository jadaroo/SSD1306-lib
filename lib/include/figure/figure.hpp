#pragma once

#include "display/display.hpp"
#include <cstdint>
#include <utility>

namespace SSD1306{
  
class Figure {
  public:
    Figure(std::pair<uint8_t, uint8_t> pos) : pos_(pos){};
    virtual void draw(DISPLAY &display) = 0;
    virtual void move_to(std::pair<uint8_t, uint8_t> new_pos, DISPLAY &display) = 0;
    virtual void move_by(std::pair<uint8_t, uint8_t> shift, DISPLAY &display) = 0;
    virtual void delete_figure(DISPLAY &display) = 0;
    std::pair<uint8_t, uint8_t> pos() { return pos_; }
    void set_pos(std::pair<uint8_t, uint8_t> new_pos) { pos_ = new_pos;}
    virtual ~Figure() = default;
  private:
    std::pair<uint8_t, uint8_t> pos_;
};

};
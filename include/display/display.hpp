#pragma once

#include "display/rectangle.hpp"
#include "display/strings.hpp"
#include "i2c_bus/i2c_transport.hpp"
#include <cstdint>
#include <utility>

class DISPLAY{
  public:
    DISPLAY(I2CTRANSPORT *i2c);
    void init_disp();
    void add_rect(RECT &rect);
    void delete_rect(RECT &rect);
    void clear_display();
    void print_data();
    void set_pixel(std::pair<uint8_t, uint8_t> pixel);
    void clear_pixel(std::pair<uint8_t, uint8_t> pixel);
    void add_string(String &string);
    void delete_string(String &string);
  private:
    uint8_t display_[1024]{0};
    I2CTRANSPORT *i2c_;
    void disp_on();
    void add_char(std::pair<uint8_t, uint8_t> pos, char symbol);
    void delete_char(std::pair<uint8_t, uint8_t> pos, char symbol);
};
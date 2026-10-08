#pragma once

#include "display/rectangle.hpp"
#include "display/strings.hpp"
#include "i2c_bus/i2c_transport.hpp"
#include <cstdint>
#include <utility>

constexpr uint8_t glyph_width = 5; //	font property
constexpr uint8_t glyph_height = 8; // 	font property
constexpr uint8_t page_height = 8; 
constexpr uint8_t char_advance = 7;
constexpr uint8_t line_advance = 9;
constexpr uint8_t display_on = 0xA4;
constexpr uint8_t disp_normal = 0xAF;
constexpr uint8_t enable_charge_pump[2] = {0x8D, 0b00010100};
constexpr uint8_t set_hor_mem_mode[2] = {0x20, 0x20};
constexpr uint8_t disp_height = 64;
constexpr uint8_t disp_width = 128;

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
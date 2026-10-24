#pragma once

#include "i2c_bus/i2c_transport.hpp"
#include "point.hpp"
#include "strings.hpp"
#include <cstdint>

constexpr uint8_t glyph_width = 5;  //	font property
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

class DISPLAY
{
public:
  DISPLAY(I2CTRANSPORT *i2c);
  void init_disp();
  void clear_display();
  void update_display();
  void set_pixel(Point pixel);
  void clear_pixel(Point pixel);
  void fill_hor_line(int16_t x0, int16_t x1, int16_t y);
  void clear_hor_line(int16_t x0, int16_t x1, int16_t y);
  void add_string(SSD1306::String &string);
  void delete_string(SSD1306::String &string);
  void change_string(SSD1306::String &string, const char *new_string, uint8_t len);
private:
  uint8_t display_[1024]{0};
  uint8_t dirty_ = 0;
  I2CTRANSPORT *i2c_;
  void disp_on();
  void mark_dirty(uint8_t page);
  void add_char(Point pos, char symbol);
  void delete_char(Point pos, char symbol);
};

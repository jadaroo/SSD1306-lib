#include "display/display.hpp"
#include "commands.hpp"
#include "display/strings.hpp"
#include "glcdfont.hpp"
#include "i2c_bus/bus.hpp"
#include "i2c_bus/i2c_transport.hpp"
#include <cstdint>
#include <cstring>
#include <iostream>

DISPLAY::DISPLAY(I2CTRANSPORT *i2c) : i2c_(i2c) {
  if(i2c_ == nullptr) {
    std::cout << "DISPLAY: i2c transport is nullptr" << "\n";
  }
}

namespace {
bool error_check(Status operation, const char *where) {
  if(operation == Status::OK) {
    return true;
  }

  std::cout << where << ": ";
  if(operation == Status::NACK) {
    std::cout << "NACK" << "\n";
  } else if(operation == Status::TIMEOUT) {
    std::cout << "TIMEOUT" << "\n";
  } else if(operation == Status::ERROR) {
    std::cout << "ERROR" << "\n";
  } else {
    std::cout << "unknown status " << +static_cast<uint8_t>(operation) << "\n";
  }
  return false;
}

bool out_of_bounce(Point pos) {
  return ((pos.first < 0) || (pos.second < 0) || (pos.first >= disp_width) || (pos.second >= disp_height));
}

bool clip_hor_line(int16_t &x0, int16_t &x1, int16_t y) {
  if(y < 0 || y >= disp_height) return false;
  if(x0 < 0) x0 = 0;
  if(x1 > disp_width - 1) x1 = disp_width - 1;
  return x0 <= x1;
}
}; // namespace

void DISPLAY::mark_dirty(uint8_t page) {
  dirty_ |= (1U << page);
}

void DISPLAY::disp_on() {
  if(i2c_ == nullptr) {
    std::cout << "disp_on: i2c transport is nullptr" << "\n";
    return;
  }
  error_check(i2c_->write_command(&display_on, 1), "disp_on");
}

void DISPLAY::init_disp() {
  if(i2c_ == nullptr) {
    std::cout << "init_disp: i2c transport is nullptr" << "\n";
    return;
  }
  error_check(i2c_->write_command(enable_charge_pump, 2), "init_disp: charge pump");
  error_check(i2c_->write_command(set_hor_mem_mode, 2), "init_disp: memory mode");
  error_check(i2c_->write_command(&disp_normal, 1), "init_disp: display on");
  clear_display();
}

void DISPLAY::set_pixel(Point pixel) {
  if(out_of_bounce(pixel))
    return;
  uint8_t page = pixel.second / page_height;
  size_t byte_idx = (page * disp_width) + pixel.first;
  uint8_t bit = pixel.second % 8;

  display_[byte_idx] |= (1U << bit);

  mark_dirty(page);
}

void DISPLAY::clear_pixel(Point pixel) {
  if(out_of_bounce(pixel))
    return;
  uint8_t page = pixel.second / page_height;
  uint16_t byte_idx = (page * disp_width) + pixel.first;
  uint8_t bit = pixel.second % 8;

  display_[byte_idx] &= static_cast<uint8_t>(~(1U << bit));

  mark_dirty(page);
}

void DISPLAY::fill_hor_line(int16_t x0, int16_t x1, int16_t y) {
  if(!clip_hor_line(x0, x1, y)) return;
  for(int16_t x = x0; x <= x1; x++) {
    set_pixel({x, y});
  }
}

void DISPLAY::clear_hor_line(int16_t x0, int16_t x1, int16_t y) {
  if(!clip_hor_line(x0, x1, y)) return;
  for(int16_t x = x0; x <= x1; x++) {
    clear_pixel({x, y});
  }
}

void DISPLAY::print_data() {
  if(i2c_ == nullptr) {
    std::cout << "print_data: i2c transport is nullptr" << "\n";
    return;
  }
  for(uint8_t i = 0; i < 8; i++) {
    uint8_t data[128];
    if(dirty_ & (1U << i)) {
      uint16_t page_index = i * page_size;
      memcpy(data, &display_[page_index], page_size);

      if(error_check(i2c_->write_page(i, data), "print_data")) {
        dirty_ &= ~(1U << i);
      } else {
        std::cout << "print_data: page " << +i << " not sent" << "\n";
      }
    }
  }
}

void DISPLAY::clear_display() {
  if(i2c_ == nullptr) {
    std::cout << "clear_display: i2c transport is nullptr" << "\n";
    return;
  }
  for(uint8_t i = 0; i < 8; i++) {
    uint8_t data[128];
    memset(data, 0, 128);
    if(!error_check(i2c_->write_page(i, data), "clear_display")) {
      std::cout << "clear_display: page " << +i << " not cleared" << "\n";
    }
  }
}

void DISPLAY::add_char(Point pos, char symbol) {
  size_t char_idx = static_cast<uint8_t>(symbol) * glyph_width;
  if(char_idx + glyph_width > sizeof(font)) {
    std::cout << "unsupported char code " << +static_cast<uint8_t>(symbol) << "\n";
    return;
  }
  for(size_t i = char_idx; i < char_idx + glyph_width; i++) {
    uint8_t column = font[i];
    for(uint8_t j = 0; j < glyph_height; j++) {
      if((column >> j & 1) == 1) {
        set_pixel({pos.first + (i - char_idx), pos.second + j});
      }
    }
  }
}

void DISPLAY::delete_char(Point pos, char symbol) {
  size_t char_idx = static_cast<uint8_t>(symbol) * glyph_width;
  if(char_idx + glyph_width > sizeof(font)) {
    std::cout << "unsupported char code " << +static_cast<uint8_t>(symbol) << "\n";
    return;
  }
  for(size_t i = char_idx; i < char_idx + glyph_width; i++) {
    uint8_t column = font[i];
    for(uint8_t j = 0; j < glyph_height; j++) {
      if((column >> j & 1) == 1) {
        clear_pixel({pos.first + (i - char_idx), pos.second + j});
      }
    }
  }
}

void DISPLAY::add_string(SSD1306::String &string) {
  Point char_pos = string.point_;
  for(uint8_t i = 0; i < string.len_; i++) {
    add_char(char_pos, string.string_[i]);
    if(char_pos.first <= disp_width - char_advance)
      char_pos.first += char_advance;
    if(char_pos.first > disp_width - char_advance) {
      char_pos.first = 1;
      char_pos.second += line_advance;
    }
  }
}

void DISPLAY::delete_string(SSD1306::String &string) {
  Point char_pos = string.point_;
  for(uint8_t i = 0; i < string.len_; i++) {
    delete_char(char_pos, string.string_[i]);
    if(char_pos.first <= disp_width - char_advance)
      char_pos.first += char_advance;
    if(char_pos.first > disp_width - char_advance) {
      char_pos.first = 1;
      char_pos.second += line_advance;
    }
  }
}

void DISPLAY::change_string(SSD1306::String &string, const char *new_string, uint8_t len) {
  if(len > max_string_len) {
    std::cout << "Too long string" << "\n";
    return;
  }
  delete_string(string);
  memcpy(string.string_, new_string, len);
  string.len_ = len;
  add_string(string);
}

#include "display/rectangle.hpp"
#include "display/strings.hpp"
#include "i2c_bus/bus.hpp"
#include "i2c_bus/i2c_transport.hpp"
#include "display/display.hpp"
#include "glcdfont.hpp"
#include <cstdint>
#include <cstring>
#include <iostream>
#include <utility>

DISPLAY::DISPLAY(I2CTRANSPORT *i2c) : i2c_(i2c){}

namespace {
  void error_check(Status &operation){
    if(operation == Status::NACK){
      std::cout << "NACK" << "\n";
    } else if(operation == Status::ERROR){
      std::cout << "ERROR" << "\n";
    } else if(operation == Status::OK){
      std::cout << "OK" << "\n";
    } else {
      std::cout << "UB" << "\n";
    }
  }

  bool out_of_bounce(std::pair<uint8_t, uint8_t> pos){
    return ((pos.first >= disp_width) || (pos.second >= disp_width));
  }
};

void DISPLAY::disp_on(){
  Status disp = i2c_->write_command(&display_on, 1);
  error_check(disp);
}

void DISPLAY::init_disp(){
  Status init[3] = {i2c_->write_command(enable_charge_pump, 2), i2c_->write_command(set_hor_mem_mode, 2), i2c_->write_command(&disp_normal, 1)};

  for(auto& x : init){
    error_check(x);
  }
}

void DISPLAY::set_pixel(std::pair<uint8_t, uint8_t> pixel){
  if(out_of_bounce(pixel)) return;
  uint8_t page = pixel.second / page_height;
  size_t byte_idx = (page * disp_width) + pixel.first;
  uint8_t bit = pixel.second % 8;

  display_[byte_idx] |= (1 << bit);
}

void DISPLAY::clear_pixel(std::pair<uint8_t, uint8_t> pixel){
  if(out_of_bounce(pixel)) return;
  uint8_t page = pixel.second / page_height;
  uint16_t byte_idx = (page * disp_width) + pixel.first;
  uint8_t bit = pixel.second % 8;

  display_[byte_idx] &= static_cast<uint8_t>(~(1 << bit));
}

void DISPLAY::print_data(){
  Status operation = i2c_->write_data(display_);
  error_check(operation);
  disp_on();
}

void DISPLAY::clear_display(){
  memset(display_, 0, 1024);
}

void DISPLAY::add_rect(SSD1306::Rect &rect){
  for(uint8_t i = rect.pos_.second; i < (rect.pos_.second + rect.height_); i++){
    for(uint8_t j = rect.pos_.first; j < (rect.pos_.first + rect.width_); j++){
     set_pixel({j, i});
    }
  }
}

void DISPLAY::delete_rect(SSD1306::Rect &rect){
  for(uint8_t i = rect.pos_.second; i < (rect.pos_.second + rect.height_); i++){
    for(uint8_t j = rect.pos_.first; j < (rect.pos_.first + rect.width_); j++){
      clear_pixel({j, i});
    }
  }
}

void DISPLAY::add_char(std::pair<uint8_t, uint8_t> pos, char symbol){
  size_t char_idx = symbol * glyph_width;
  for(size_t i = char_idx; i < char_idx + glyph_width; i++){
    uint8_t column = font[i];
    for(uint8_t j = 0; j < glyph_height; j++){
      if((column >> j & 1) == 1){
        set_pixel({pos.first + (i - char_idx), pos.second + j});
      }
    }
  }
}

void DISPLAY::delete_char(std::pair<uint8_t, uint8_t> pos, char symbol){
  size_t char_idx = symbol * glyph_width;
  for(size_t i = char_idx; i < char_idx + glyph_width; i++){
    uint8_t column = font[i];
    for(uint8_t j = 0; j < glyph_height; j++){
      if((column >> j & 1) == 1){
        clear_pixel({pos.first + (i - char_idx), pos.second + j});
      }
    }
  }
}

void DISPLAY::add_string(SSD1306::String &string){
  std::pair<uint8_t, uint8_t> char_pos = string.point_;
  for(const auto& elem : string.string_){
    add_char(char_pos, elem);
    if(char_pos.first <= disp_width - char_advance) char_pos.first += char_advance;
    if(char_pos.first > disp_width - char_advance){
      char_pos.first = 1;
      char_pos.second += line_advance;
    }
  }
}

void DISPLAY::delete_string(SSD1306::String &string){
  std::pair<uint8_t, uint8_t> char_pos = string.point_;
  for(const auto& elem : string.string_){
    delete_char(char_pos, elem);
    if(char_pos.first <= disp_width - char_advance) char_pos.first += char_advance;
    if(char_pos.first > disp_width - char_advance){
      char_pos.first = 1;
      char_pos.second += line_advance;
    }
  }
}

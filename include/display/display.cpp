#include "display.hpp"
#include "display/strings.hpp"
#include "i2c_bus/bus.hpp"
#include "i2c_bus/i2c_transport.hpp"
#include "glcdfont.hpp"
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <iostream>
#include <utility>
#include "strings.hpp"

constexpr uint8_t display_on = 0xA4;
constexpr uint8_t disp_normal = 0xAF;
constexpr uint8_t enable_charge_pump[2] = {0x8D, 0b00010100};
constexpr uint8_t set_hor_mem_mode[2] = {0x20, 0x20};

DISPLAY::DISPLAY(I2CTRANSPORT *i2c) : i2c_(i2c){}

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
  uint8_t page = (pixel.second - 1) / 8;
  size_t byte_idx = (page * 128) + (pixel.first - 1);
  uint8_t bit = (pixel.second - 1) % 8;

  display_[byte_idx] |= (1 << bit); 
}

void DISPLAY::clear_pixel(std::pair<uint8_t, uint8_t> pixel){
  uint8_t page = (pixel.second - 1) / 8;
  size_t byte_idx = (page * 128) + (pixel.first - 1);
  uint8_t bit = (pixel.second - 1) % 8;

  display_[byte_idx] = (0 << bit); 
}

void DISPLAY::print_data(){
  i2c_->write_data(display_);
  disp_on();
}

void DISPLAY::clear_display(){
  memset(display_, 0, 1024);
}

void DISPLAY::add_rect(RECT &rect){
  for(uint8_t i = rect.pos_.second; i < (rect.pos_.second + rect.height_); i++){
    for(uint8_t j = rect.pos_.first; j < (rect.pos_.first + rect.width_); j++){
      set_pixel({j, i});
    }
  }
}

void DISPLAY::delete_rect(RECT &rect){
  for(uint8_t i = rect.pos_.second; i < (rect.pos_.second + rect.height_); i++){
    for(uint8_t j = rect.pos_.first; j < (rect.pos_.first + rect.width_); j++){
      clear_pixel({j, i});
    }
  }
}

void DISPLAY::add_char(std::pair<uint8_t, uint8_t> pos, char symbol){
  size_t char_idx = symbol * 5;
  for(size_t i = char_idx; i < char_idx + 5; i++){
    uint8_t column = font[i];
    for(uint8_t j = 0; j < 8; j++){
      if((column >> j & 1) == 1){
        set_pixel({pos.first + (i - char_idx), pos.second + j});
      }
    }
  }
}

void DISPLAY::delete_char(std::pair<uint8_t, uint8_t> pos, char symbol){
  size_t char_idx = symbol * 5;
  for(size_t i = char_idx; i < char_idx + 5; i++){
    uint8_t column = font[i];
    for(uint8_t j = 0; j < 8; j++){
      if((column >> j & 1) == 1){
        clear_pixel({pos.first + (i - char_idx), pos.second + j});
      }
    }
  }
}

void DISPLAY::add_string(String &string){
  std::pair<uint8_t, uint8_t> char_pos = string.point_;
  for(const auto& elem : string.string_){
    add_char(char_pos, elem);
    if(char_pos.first <= 121) char_pos.first += 7;  
    if(char_pos.first > 121){
      char_pos.first = 1;
      char_pos.second += 9;
    }
  }
}

void DISPLAY::delete_string(String &string){
  std::pair<uint8_t, uint8_t> char_pos = string.point_;
  for(const auto& elem : string.string_){
    delete_char(char_pos, elem);
    if(char_pos.first <= 121) char_pos.first += 7;  
    if(char_pos.first > 121){
      char_pos.first = 1;
      char_pos.second += 9;
    }
  }
}

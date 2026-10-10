#include "figure/circle.hpp"
#include "display/display.hpp"
#include "figure/figure.hpp"
#include <cstdint>
#include <utility>

SSD1306::Circle::Circle(std::pair<uint8_t, uint8_t> center_pos, uint8_t rad) : Figure(center_pos), rad_(rad){}

namespace {
  void draw_horizontal_line(uint8_t px1, uint8_t px2, uint8_t y, DISPLAY &display){
    for(uint8_t i = px1; i <= px2; i++){
      display.set_pixel({i, y});
    }
  }

  void delete_horizontal_line(uint8_t px1, uint8_t px2, uint8_t y, DISPLAY &display){
    for(uint8_t i = px1; i <= px2; i++){
      display.clear_pixel({i, y});
    }
  }
};

void SSD1306::Circle::draw(DISPLAY &display){
  uint8_t x = rad_;
  uint8_t y = 0;

  // Printing the initial point on the axes
  // after translation
  display.set_pixel({x + pos().first, y + pos().second});

  // When radius is zero only a single
  // point will be printed
  if (rad_ > 0) {
    display.set_pixel({x + pos().first, -y + pos().second});
    display.set_pixel({y + pos().first, x + pos().second});
    display.set_pixel({-y + pos().first, x + pos().second});

    draw_horizontal_line(-x + pos().first, x + pos().first, y + pos().second, display);
  }

  // Initialising the value of P
  auto decision = 1 - rad_;
  while (x > y){
    y++;

    // Mid-point is inside or on the perimeter
    if (decision <= 0){
      decision = decision + (2 * y) + 1;
    } else{ // Mid-point is outside the perimeter
      x--;
      decision = decision + (2 * y) - (2 * x) + 1;
    }

    // All the perimeter points have already been printed
    if (x < y) break;

    // Printing the generated point and its reflection
    // in the other octants after translation
    draw_horizontal_line(-x + pos().first, x + pos().first, y + pos().second, display);
    draw_horizontal_line(-x + pos().first, x + pos().first, -y + pos().second, display);
    
    // If the generated point is on the line x = y then
    // the perimeter points have already been printed
    if (x != y){
      draw_horizontal_line(-y + pos().first, y + pos().first, x + pos().second, display);
      draw_horizontal_line(-y + pos().first, y + pos().first, -x + pos().second, display);
    }
  }
}

void SSD1306::Circle::delete_figure(DISPLAY &display){
  uint8_t x = rad_;
  uint8_t y = 0;

  // Printing the initial point on the axes
  // after translation
  display.clear_pixel({x + pos().first, y + pos().second});

  // When radius is zero only a single
  // point will be printed
  if (rad_ > 0) {
    display.clear_pixel({x + pos().first, -y + pos().second});
    display.clear_pixel({y + pos().first, x + pos().second});
    display.clear_pixel({-y + pos().first, x + pos().second});

    delete_horizontal_line(-x + pos().first, x + pos().first, y + pos().second, display);
  }

  // Initialising the value of P
  auto decision = 1 - rad_;
  while (x > y){
    y++;

    // Mid-point is inside or on the perimeter
    if (decision <= 0){
      decision = decision + (2 * y) + 1;
    } else{ // Mid-point is outside the perimeter
      x--;
      decision = decision + (2 * y) - (2 * x) + 1;
    }

    // All the perimeter points have already been printed
    if (x < y) break;

    // Printing the generated point and its reflection
    // in the other octants after translation
    delete_horizontal_line(-x + pos().first, x + pos().first, y + pos().second, display);
    delete_horizontal_line(-x + pos().first, x + pos().first, -y + pos().second, display);
    
    // If the generated point is on the line x = y then
    // the perimeter points have already been printed
    if (x != y){
      delete_horizontal_line(-y + pos().first, y + pos().first, x + pos().second, display);
      delete_horizontal_line(-y + pos().first, y + pos().first, -x + pos().second, display);
    }
  }
}

void SSD1306::Circle::move_to(std::pair<uint8_t, uint8_t> new_pos, DISPLAY &display){
  delete_figure(display);
  set_pos(new_pos);
  draw(display);
}

void SSD1306::Circle::move_by(std::pair<uint8_t, uint8_t> shift, DISPLAY &display){
  std::pair<uint8_t, uint8_t> new_pos = {pos().first + shift.first, pos().second + shift.second};
  move_to(new_pos, display);
}

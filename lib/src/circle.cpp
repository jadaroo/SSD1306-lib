#include "figure/circle.hpp"
#include "display/display.hpp"
#include "figure/figure.hpp"
#include <cstdint>

SSD1306::Circle::Circle(Point center_pos, uint8_t rad) : Figure(center_pos), rad_(rad) {}

// Midpoint circle algorithm, filled with horizontal spans.
// The display clips each line, so the circle may be partly off screen.
void SSD1306::Circle::draw(DISPLAY &display) {
  int16_t cx = pos().first;
  int16_t cy = pos().second;

  // x and y are offsets from the centre, so they are never negative
  uint8_t x = rad_;
  uint8_t y = 0;

  // Centre row plus the top and bottom points on the vertical axis
  display.fill_hor_line(cx - x, cx + x, cy);
  display.fill_hor_line(cx, cx, cy - x);
  display.fill_hor_line(cx, cx, cy + x);

  // Initialising the value of P
  auto decision = 1 - rad_;
  while(x > y) {
    y++;

    // Mid-point is inside or on the perimeter
    if(decision <= 0) {
      decision = decision + (2 * y) + 1;
    } else { // Mid-point is outside the perimeter
      x--;
      decision = decision + (2 * y) - (2 * x) + 1;
    }

    // All the perimeter points have already been printed
    if(x < y) break;

    // Rows above and below the centre, reflected across the octants
    display.fill_hor_line(cx - x, cx + x, cy + y);
    display.fill_hor_line(cx - x, cx + x, cy - y);

    // If the generated point is on the line x = y then
    // these rows have already been printed
    if(x != y) {
      display.fill_hor_line(cx - y, cx + y, cy + x);
      display.fill_hor_line(cx - y, cx + y, cy - x);
    }
  }
}

void SSD1306::Circle::clear_figure(DISPLAY &display) {
  int16_t cx = pos().first;
  int16_t cy = pos().second;

  // x and y are offsets from the centre, so they are never negative
  uint8_t x = rad_;
  uint8_t y = 0;

  // Centre row plus the top and bottom points on the vertical axis
  display.clear_hor_line(cx - x, cx + x, cy);
  display.clear_hor_line(cx, cx, cy - x);
  display.clear_hor_line(cx, cx, cy + x);

  // Initialising the value of P
  auto decision = 1 - rad_;
  while(x > y) {
    y++;

    // Mid-point is inside or on the perimeter
    if(decision <= 0) {
      decision = decision + (2 * y) + 1;
    } else { // Mid-point is outside the perimeter
      x--;
      decision = decision + (2 * y) - (2 * x) + 1;
    }

    // All the perimeter points have already been printed
    if(x < y) break;

    // Rows above and below the centre, reflected across the octants
    display.clear_hor_line(cx - x, cx + x, cy + y);
    display.clear_hor_line(cx - x, cx + x, cy - y);

    // If the generated point is on the line x = y then
    // these rows have already been printed
    if(x != y) {
      display.clear_hor_line(cx - y, cx + y, cy + x);
      display.clear_hor_line(cx - y, cx + y, cy - x);
    }
  }
}

void SSD1306::Circle::move_to(Point new_pos, DISPLAY &display) {
  clear_figure(display);
  set_pos(new_pos);
  draw(display);
}

void SSD1306::Circle::move_by(Point shift, DISPLAY &display) {
  Point new_pos = {pos().first + shift.first, pos().second + shift.second};
  move_to(new_pos, display);
}

// Bouncing ball: a frame drawn from thin rectangles and a circle that
// bounces inside it at a fixed frame rate.

#include "display/display.hpp"
#include "example_board.hpp"
#include "figure/circle.hpp"
#include "figure/rect.hpp"
#include "i2c_bus/i2c_transport.hpp"
#include <pico/stdlib.h>

namespace {
constexpr uint32_t frame_time_ms = 20; // ~50 FPS
constexpr int16_t radius = 5;

// Limits for the ball centre: keep a one pixel gap to the frame, because
// erasing the ball would otherwise erase the frame pixels it touches
constexpr int16_t min_x = 2 + radius;
constexpr int16_t max_x = disp_width - 3 - radius;
constexpr int16_t min_y = 2 + radius;
constexpr int16_t max_y = disp_height - 3 - radius;

// Advances one coordinate and reverses its velocity when it hits a limit
int16_t bounce(int16_t pos, int16_t &velocity, int16_t min, int16_t max) {
  int16_t next = pos + velocity;
  if(next < min || next > max) {
    velocity = -velocity;
    if(next < min) {
      next = min;
    } else {
      next = max;
    }
  }
  return next;
}
}; // namespace

int main() {
  stdio_init_all();
  example::init_i2c();

  I2CTRANSPORT i2c(i2c_default, example::display_address, example::i2c_timeout_us);
  DISPLAY display(&i2c);
  display.init_disp();

  // Frame: four 1 px wide rectangles along the screen edges
  SSD1306::Rect frame[] = {
      {{0, 0}, 1, disp_width},               // top
      {{0, disp_height - 1}, 1, disp_width}, // bottom
      {{0, 0}, disp_height, 1},              // left
      {{disp_width - 1, 0}, disp_height, 1}, // right
  };
  for(auto &side : frame) {
    side.draw(display);
  }

  SSD1306::Circle ball({disp_width / 2, disp_height / 2}, radius);
  ball.draw(display);
  display.update_display();

  int16_t velocity_x = 2;
  int16_t velocity_y = 1;

  absolute_time_t next_frame = get_absolute_time();
  while(true) {
    Point pos = ball.pos();
    Point next = {bounce(pos.first, velocity_x, min_x, max_x),
                  bounce(pos.second, velocity_y, min_y, max_y)};

    ball.move_to(next, display);
    display.update_display();

    // sleep_until keeps a steady frame rate regardless of how long drawing took
    next_frame = delayed_by_ms(next_frame, frame_time_ms);
    sleep_until(next_frame);
  }
}

#pragma once

#include "display/display.hpp"
#include "display/point.hpp"
#include "figure/figure.hpp"
#include <pico/time.h>
#include <pico/types.h>

namespace SSD1306 {

class Animator {
  public:
    Animator(SSD1306::Figure &figure, DISPLAY &display);
    void move_animated(Point target, uint step_time_us);
    void update(absolute_time_t now);
  private:
    DISPLAY &display_;
    volatile bool is_animating_ = false;
    absolute_time_t step_time_;
    Point target_ = {0, 0};
    SSD1306::Figure &figure_;
    absolute_time_t last_step;
};

};
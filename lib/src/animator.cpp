#include "animator/animator.hpp"
#include "display/display.hpp"
#include <pico/time.h>

SSD1306::Animator::Animator(SSD1306::Figure &figure, DISPLAY &display) : figure_(figure), display_(display), last_step(0) {}

void SSD1306::Animator::move_animated(Point target, uint step_time) {
  last_step = get_absolute_time();
  target_ = target;
  step_time_ = step_time;
  is_animating_ = true;
}

void SSD1306::Animator::update(absolute_time_t now) {
  if(figure_.pos() == target_) {
    is_animating_ = false;
  }

  if(absolute_time_diff_us(last_step, now) >= step_time_ && is_animating_) {
    if(figure_.pos().first < target_.first) {
      figure_.move_by({1, 0}, display_);
    } else if(figure_.pos().first > target_.first) {
      figure_.move_by({-1, 0}, display_);
    } else if(figure_.pos().second < target_.second) {
      figure_.move_by({0, 1}, display_);
    } else if(figure_.pos().second > target_.second) {
      figure_.move_by({0, -1}, display_);
    }

    last_step += step_time_;
  }
}

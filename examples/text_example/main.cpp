// Text rendering: a static title, a long string that wraps on its own,
// and a value that is redrawn once per second.

#include "display/display.hpp"
#include "display/strings.hpp"
#include "example_board.hpp"
#include "i2c_bus/i2c_transport.hpp"
#include <cstdio>
#include <pico/stdlib.h>
#include <string_view>

int main() {
  stdio_init_all();
  example::init_i2c();

  I2CTRANSPORT i2c(i2c_default, example::display_address, example::i2c_timeout_us);
  DISPLAY display(&i2c);
  display.init_disp();

  SSD1306::String title("== TEXT DEMO ==", {12, 0});
  display.add_string(title);

  // Text that runs past the right edge continues on the next line
  SSD1306::String paragraph("Long strings wrap to the next line on their own.", {1, 12});
  display.add_string(paragraph);

  // String only references its characters, so the buffer must outlive it
  char uptime_text[20];
  SSD1306::String uptime("", {1, 48});

  uint32_t seconds = 0;
  absolute_time_t next_tick = get_absolute_time();
  while(true) {
    // Erase the old value while its buffer still holds the old characters
    display.delete_string(uptime);

    int len = snprintf(uptime_text, sizeof(uptime_text), "Uptime: %lu s", static_cast<unsigned long>(seconds));
    uptime = SSD1306::String(std::string_view(uptime_text, len), {1, 48});
    display.add_string(uptime);

    // Only the pages that changed are sent
    display.print_data();

    seconds++;
    next_tick = delayed_by_ms(next_tick, 1000);
    sleep_until(next_tick);
  }
}

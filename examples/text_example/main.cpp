// Text rendering: a static title, a long string that wraps on its own,
// and text that is replaced once per second.

#include "display/display.hpp"
#include "display/strings.hpp"
#include "example_board.hpp"
#include "i2c_bus/i2c_transport.hpp"
#include <cstdio>
#include <cstring>
#include <pico/stdlib.h>

int main() {
  stdio_init_all();
  example::init_i2c();

  I2CTRANSPORT i2c(i2c_default, example::display_address, example::i2c_timeout_us);
  DISPLAY display(&i2c);
  display.init_disp();

  // String copies the text into its own buffer
  SSD1306::String title({12, 0}, "== TEXT DEMO ==", 15);
  display.add_string(title);

  // Text that runs past the right edge continues on the next line
  const char *paragraph_text = "Long strings wrap to the next line on their own.";
  SSD1306::String paragraph({1, 12}, paragraph_text, strlen(paragraph_text));
  display.add_string(paragraph);

  // Start empty, the loop fills them in
  SSD1306::String status({1, 42}, "", 0);
  SSD1306::String uptime({1, 54}, "", 0);

  uint32_t seconds = 0;
  absolute_time_t next_tick = get_absolute_time();
  while(true) {
    // change_string erases the old text first, so the new one may be shorter
    if(seconds % 2 == 0) {
      display.change_string(status, "State: tick", 11);
    } else {
      display.change_string(status, "State: tock!!", 13);
    }

    char uptime_text[20];
    int len = snprintf(uptime_text, sizeof(uptime_text), "Uptime: %lu s", static_cast<unsigned long>(seconds));
    display.change_string(uptime, uptime_text, len);

    // Only the pages that changed are sent
    display.print_data();

    seconds++;
    next_tick = delayed_by_ms(next_tick, 1000);
    sleep_until(next_tick);
  }
}

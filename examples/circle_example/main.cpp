#include "figure/circle.hpp"
#include "i2c_bus/i2c_transport.hpp"
#include "display/display.hpp"
#include <hardware/gpio.h>
#include <hardware/irq.h>
#include <hardware/regs/intctrl.h>
#include <hardware/structs/io_bank0.h>
#include <hardware/timer.h>
#include <pico/error.h>
#include <pico/platform/common.h>
#include <pico/stdio.h>
#include <pico/time.h>
#include <pico/types.h>
#include "hardware/i2c.h"

constexpr uint8_t opcode = 0b0111100;

void init_i2c_default(){
  i2c_init(i2c_default, 400 * 1000);

  gpio_set_function(PICO_DEFAULT_I2C_SCL_PIN, GPIO_FUNC_I2C);
  gpio_set_function(PICO_DEFAULT_I2C_SDA_PIN, GPIO_FUNC_I2C);

  gpio_pull_up(PICO_DEFAULT_I2C_SCL_PIN);
  gpio_pull_up(PICO_DEFAULT_I2C_SDA_PIN);
}

int main(){
  stdio_init_all();
  init_i2c_default();

  I2CTRANSPORT i2c(i2c_default, opcode, 400 * 1000);
  DISPLAY display(&i2c);

  display.init_disp();
  
  SSD1306::Circle circle({10, 10}, 10);
  circle.draw(display);
  display.print_data();
  
  while(circle.pos().first != 110 && circle.pos().second != 50){
    while(circle.pos().second != 50){
      circle.move_by({0, 10}, display);
      display.print_data();
      sleep_ms(70);
    }
    circle.move_to({circle.pos().first + 10, 10}, display);
  }
  
  while (true) { 
    tight_loop_contents();
  }
}
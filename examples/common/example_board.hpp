#pragma once

// Board setup shared by all examples.
//
// Wiring (Pico / Pico W default I2C0 pins):
//   GP4 (SDA) -> SDA
//   GP5 (SCL) -> SCL
//   3V3       -> VCC
//   GND       -> GND

#include <cstdint>
#include <hardware/gpio.h>
#include <hardware/i2c.h>
#include <pico/stdlib.h>

namespace example {

constexpr uint8_t display_address = 0x3C;
constexpr uint32_t i2c_baudrate = 1000 * 1000;
constexpr uint32_t i2c_timeout_us = 100 * 1000;

inline void init_i2c() {
  i2c_init(i2c_default, i2c_baudrate);

  gpio_set_function(PICO_DEFAULT_I2C_SDA_PIN, GPIO_FUNC_I2C);
  gpio_set_function(PICO_DEFAULT_I2C_SCL_PIN, GPIO_FUNC_I2C);

  gpio_pull_up(PICO_DEFAULT_I2C_SDA_PIN);
  gpio_pull_up(PICO_DEFAULT_I2C_SCL_PIN);
}

}; // namespace example

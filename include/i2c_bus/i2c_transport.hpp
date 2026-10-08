#pragma once

#include "bus.hpp"
#include "hardware/i2c.h"

#include <cstddef>
#include <cstdint>

constexpr uint8_t command_will_be_send = 0x00;
constexpr uint8_t data_will_be_send = 0x40;
constexpr uint8_t address_reboot = 0x21;
inline constexpr uint16_t framebuffer_size = 1024;
inline constexpr uint16_t size_of_transmitter = 1025;

class I2CTRANSPORT {
  public:
    I2CTRANSPORT(i2c_inst_t *i2c, uint8_t address, uint32_t timeout_us);
    Status write_command(const uint8_t *command, size_t len);
    Status write_data(uint8_t *data);
  private:
    i2c_inst_t *i2c_;
    uint8_t address_;
    uint32_t timeout_us_;
};
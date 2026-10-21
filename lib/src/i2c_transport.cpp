#include "i2c_bus/i2c_transport.hpp"
#include "commands.hpp"
#include "i2c_bus/bus.hpp"
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <hardware/i2c.h>
#include <iostream>
#include <pico/error.h>

I2CTRANSPORT::I2CTRANSPORT(i2c_inst_t *i2c, uint8_t address, uint32_t timeout_us) : i2c_(i2c), address_(address), timeout_us_(timeout_us) {
  if(i2c_ == nullptr) {
    std::cout << "I2CTRANSPORT: i2c instance is nullptr" << "\n";
  }
}

namespace {
Status to_status(int send, size_t expected) {
  if(send == PICO_ERROR_GENERIC) {
    return Status::NACK;
  } else if(send == PICO_ERROR_TIMEOUT) {
    return Status::TIMEOUT;
  } else if(send < 0 || static_cast<size_t>(send) != expected) {
    std::cout << "I2CTRANSPORT: sent " << send << " of " << expected << " bytes" << "\n";
    return Status::ERROR;
  }
  return Status::OK;
}
}; // namespace

Status I2CTRANSPORT::write_command(const uint8_t *commands, size_t len) {
  if(i2c_ == nullptr || commands == nullptr) {
    std::cout << "write_command: nullptr argument" << "\n";
    return Status::ERROR;
  }
  if(len == 0 || len + 1 > size_of_transmitter_commands) {
    std::cout << "write_command: invalid length " << len << "\n";
    return Status::ERROR;
  }

  uint8_t array[size_of_transmitter_commands]{};
  array[0] = command_will_be_send;
  memcpy(&array[1], commands, len);
  int send = i2c_write_timeout_us(i2c_, address_, array, len + 1, false, timeout_us_);

  return to_status(send, len + 1);
}

Status I2CTRANSPORT::write_page(uint8_t page, uint8_t *data) {
  if(i2c_ == nullptr || data == nullptr) {
    std::cout << "write_page: nullptr argument" << "\n";
    return Status::ERROR;
  }
  if(page >= framebuffer_size / page_size) {
    std::cout << "write_page: page " << +page << " out of range" << "\n";
    return Status::ERROR;
  }

  uint8_t set_page[3] = {set_page_command, page, page};
  Status status = write_command(set_page, 3);
  if(status != Status::OK) {
    std::cout << "write_page: set page " << +page << " failed" << "\n";
    return status;
  }

  uint8_t set_column[3] = {set_column_address, 0, 127};
  status = write_command(set_column, 3);
  if(status != Status::OK) {
    std::cout << "write_page: set column failed" << "\n";
    return status;
  }

  uint8_t array[size_of_transmitter_data];
  array[0] = data_will_be_send;
  memcpy(&array[1], data, page_size);
  int send = i2c_write_timeout_us(i2c_, address_, array, size_of_transmitter_data, false, timeout_us_);

  return to_status(send, size_of_transmitter_data);
}

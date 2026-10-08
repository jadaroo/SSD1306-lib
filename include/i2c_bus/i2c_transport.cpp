#include "i2c_transport.hpp"
#include "i2c_bus/bus.hpp"
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <hardware/i2c.h>
#include <pico/error.h>

constexpr uint8_t command_will_be_send = 0x00;
constexpr uint8_t data_will_be_send = 0x40;
constexpr uint8_t address_reboot = 0x21;

I2CTRANSPORT::I2CTRANSPORT(i2c_inst_t *i2c, uint8_t address, uint32_t timeout_us) : i2c_(i2c), address_(address), timeout_us_(timeout_us){}

Status I2CTRANSPORT::write_command(const uint8_t *commands, size_t len){
  uint8_t array[1025]{};
  array[0] = command_will_be_send;
  memcpy(&array[1], commands, len);
  int send = i2c_write_timeout_us(i2c_, address_, array, len + 1, false, timeout_us_);
 
  
  if(send == PICO_ERROR_GENERIC){
    return Status::NACK;
  } else if(send == PICO_ERROR_TIMEOUT){
    return Status::TIMEOUT;
  }

  return Status::OK;
}

Status I2CTRANSPORT::write_data(uint8_t *data){
  uint8_t array[1025]{};
  array[0] = data_will_be_send;
  memcpy(&array[1], data, 1024);
  int send = i2c_write_timeout_us(i2c_, address_, array, 1025, false, timeout_us_);

  if(send == PICO_ERROR_GENERIC){
    return Status::NACK;
  } else if(send == PICO_ERROR_TIMEOUT){
    return Status::TIMEOUT;
  }

  return Status::OK;
}

#pragma once

#include <cstdint>

constexpr uint8_t command_will_be_send = 0x00;
constexpr uint8_t data_will_be_send = 0x40;
constexpr uint8_t address_reboot = 0x21;
inline constexpr uint16_t framebuffer_size = 1024;
inline constexpr uint16_t size_of_transmitter = 1025;
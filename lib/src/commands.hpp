#pragma once

#include <cstdint>

constexpr uint8_t command_will_be_send = 0x00;
constexpr uint8_t data_will_be_send = 0x40;
constexpr uint8_t address_reboot = 0x21;
constexpr uint16_t framebuffer_size = 1024;
constexpr uint16_t size_of_transmitter_commands = 1025;
constexpr uint16_t size_of_transmitter_data = 129;
constexpr uint8_t set_page_command = 0x22;
constexpr uint8_t page_size = 128;
constexpr uint8_t set_column_address = 0x21;
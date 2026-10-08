#pragma once

#include <cstdint>

enum class Status : uint8_t { OK = 0, TIMEOUT = 1, NACK = 2, ERROR = 3 };
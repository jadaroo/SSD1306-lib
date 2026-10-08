cmake_minimum_required(VERSION 3.13)

# Built on its own (cmake -S .) vs. pulled in by another project via
# add_subdirectory(). In the second case the parent owns the Pico SDK setup.
if(CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)
    set(SSD1306_IS_TOP_LEVEL ON)
else()
    set(SSD1306_IS_TOP_LEVEL OFF)
endif()

if(SSD1306_IS_TOP_LEVEL)
    set(PICO_BOARD pico_w)
    include(pico_sdk_import.cmake)
endif()

project(SSD1306 C CXX ASM)

if(SSD1306_IS_TOP_LEVEL)
    set(CMAKE_C_STANDARD 11)
    set(CMAKE_CXX_STANDARD 17)
    set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
    pico_sdk_init()
endif()

option(SSD1306_BUILD_EXAMPLES "Build the SSD1306 example programs" ${SSD1306_IS_TOP_LEVEL})

# The driver itself: a reusable library target (ssd1306::ssd1306)
add_subdirectory(lib)

# Example / test programs that use the library
if(SSD1306_BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()

add_library(ssd1306 STATIC
    src/display.cpp
    src/i2c_transport.cpp
)
add_library(ssd1306::ssd1306 ALIAS ssd1306)

target_include_directories(ssd1306
    # PUBLIC: headers users of the library include themselves
    PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}/include
    # PRIVATE: headers only the library's own .cpp files need
    PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/src
)

# Required by the public headers (std::string_view), so consumers get it too
target_compile_features(ssd1306 PUBLIC cxx_std_17)

target_link_libraries(ssd1306
    PUBLIC
        pico_stdlib
        hardware_i2c
)

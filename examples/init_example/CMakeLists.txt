add_executable(init_example
    main.cpp
)

target_link_libraries(init_example
    PRIVATE
        ssd1306::ssd1306
)

pico_enable_stdio_usb(init_example 1)
pico_enable_stdio_uart(init_example 0)

pico_add_extra_outputs(init_example)

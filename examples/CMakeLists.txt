# Helper for example programs: an executable linked against the driver,
# with USB stdio and the usual .uf2/.hex/.bin outputs.
#
#   ssd1306_add_example(<name> <sources>...)
function(ssd1306_add_example name)
    add_executable(${name} ${ARGN})

    target_link_libraries(${name}
        PRIVATE
            ssd1306::ssd1306
    )

    pico_enable_stdio_usb(${name} 1)
    pico_enable_stdio_uart(${name} 0)

    pico_add_extra_outputs(${name})
endfunction()

# Every subdirectory with a CMakeLists.txt is an example
file(GLOB example_dirs LIST_DIRECTORIES true ${CMAKE_CURRENT_LIST_DIR}/*)
foreach(dir ${example_dirs})
    if(IS_DIRECTORY ${dir} AND EXISTS ${dir}/CMakeLists.txt)
        add_subdirectory(${dir})
    endif()
endforeach()

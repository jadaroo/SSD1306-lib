# SSD1306-lib

SSD1306 OLED driver for the Raspberry Pi Pico (RP2040) using the Pico SDK.
Minimal, dependency-free, CMake-ready.

- 128×64 framebuffer in RAM, only changed pages are sent over I2C
- Text rendering with a 5×7 font and automatic line wrapping
- Filled rectangles and circles that clip at the screen edges
- Non-blocking animator that moves figures across the screen
- I2C errors (NACK, timeout) are reported instead of hanging

## Wiring

The examples use the Pico's default I2C0 pins:

| Pico      | Display |
|-----------|---------|
| GP4 (SDA) | SDA     |
| GP5 (SCL) | SCL     |
| 3V3       | VCC     |
| GND       | GND     |

## Building

```sh
export PICO_SDK_PATH=/path/to/pico-sdk
cmake -S . -B build
cmake --build build -j
```

Built on its own, the project targets `pico_w` and builds the examples. The `.uf2` files land in `build/examples/<name>/`.

### Using it in your own project

Add the repository as a subdirectory after `pico_sdk_init()` and link against `ssd1306::ssd1306`:

```cmake
set(SSD1306_BUILD_EXAMPLES OFF)   # optional, examples are off by default here
add_subdirectory(SSD1306-lib)

target_link_libraries(my_app PRIVATE ssd1306::ssd1306)
```

The library needs C++17.

## Quick start

```cpp
#include "display/display.hpp"
#include "display/strings.hpp"
#include "i2c_bus/i2c_transport.hpp"
#include <hardware/i2c.h>
#include <pico/stdlib.h>

int main() {
  stdio_init_all();

  i2c_init(i2c_default, 400 * 1000);
  gpio_set_function(PICO_DEFAULT_I2C_SDA_PIN, GPIO_FUNC_I2C);
  gpio_set_function(PICO_DEFAULT_I2C_SCL_PIN, GPIO_FUNC_I2C);
  gpio_pull_up(PICO_DEFAULT_I2C_SDA_PIN);
  gpio_pull_up(PICO_DEFAULT_I2C_SCL_PIN);

  I2CTRANSPORT i2c(i2c_default, 0x3C, 100 * 1000); // instance, address, timeout in µs
  DISPLAY display(&i2c);
  display.init_disp();

  SSD1306::String hello("Hello, SSD1306!", {12, 28});
  display.add_string(hello);  // draws into the RAM framebuffer
  display.print_data();       // sends it to the panel

  while(true) {
    tight_loop_contents();
  }
}
```

Drawing functions only change the framebuffer. Nothing shows up until you call `print_data()`, which sends just the 8-pixel-high pages that changed since the last call.

## API overview

Coordinates are `Point = std::pair<int16_t, int16_t>`: `{x, y}`, where `{0, 0}` is the top-left corner, x runs to 127 and y to 63.

### `DISPLAY`

| Function | Description |
|----------|-------------|
| `init_disp()` | Turns the panel on and clears it. Call once. |
| `print_data()` | Sends changed pages to the panel. |
| `clear_display()` | Blanks the panel. |
| `set_pixel(p)` / `clear_pixel(p)` | Single pixel. Points off screen are ignored. |
| `fill_hor_line(x0, x1, y)` / `clear_hor_line(x0, x1, y)` | Horizontal line from `x0` to `x1` (inclusive), clipped to the screen. |
| `add_string(s)` / `delete_string(s)` | Draws or erases an `SSD1306::String`. |

### Text: `SSD1306::String`

```cpp
SSD1306::String title("Hello", {1, 0});  // text, top-left position
display.add_string(title);
```

- Each character takes 7 pixels across and each line 9 pixels down, so a line holds 18 characters. Longer text wraps onto the next line.
- `String` stores a `std::string_view`: it **does not copy** the text. The characters must stay alive as long as the `String` is used.
- To change text, call `delete_string` while the buffer still holds the old text, then write the new text and call `add_string`. `examples/text_example` shows this with a live counter.

### Figures: `SSD1306::Rect`, `SSD1306::Circle`

```cpp
SSD1306::Rect rect({10, 10}, 20, 30);  // top-left corner, height, width
SSD1306::Circle circle({64, 32}, 12);  // centre, radius

rect.draw(display);
circle.move_by({5, 0}, display);       // erase, shift, redraw
rect.move_to({-10, 40}, display);      // partly off screen is fine
circle.delete_figure(display);
```

Both shapes are filled and inherit from `SSD1306::Figure`, which also provides `pos()` and `set_pos()`.

### Animation: `SSD1306::Animator`

```cpp
SSD1306::Animator anim(rect, display);
anim.move_animated({100, 10}, 10000);  // target, µs per one-pixel step

while(true) {
  anim.update(get_absolute_time());
  display.print_data();
}
```

`update()` never blocks. It moves the figure at most one pixel per call: first along x, then along y. Call it often from your main loop; one loop can drive several animators.
The animator keeps a reference to its figure, so the figure must outlive it.

## Limitations

- **Overlapping figures:** erasing or moving a figure clears every pixel it covers, including pixels that belong to other figures or text underneath. Keep figures apart, or redraw everything after a change.
- Only 128×64 panels over I2C are supported.

## Examples

| Example | Shows |
|---------|-------|
| [`text_example`](examples/text_example/main.cpp) | A title, a long string that wraps on its own, and an uptime counter updated every second. |
| [`bouncing_ball_example`](examples/bouncing_ball_example/main.cpp) | A ball bouncing inside a frame made of rectangles at a steady 50 FPS. |

The I2C setup the examples share is in [`examples/common/example_board.hpp`](examples/common/example_board.hpp).
To add an example, create a folder in `examples/` with a `main.cpp` and a `CMakeLists.txt` containing:

```cmake
ssd1306_add_example(my_example
    main.cpp
)
```

It is picked up automatically.

## License

MIT, see [LICENSE](LICENSE).

The 5×7 font in `lib/src/glcdfont.hpp` comes from the [Adafruit GFX Library](https://github.com/adafruit/Adafruit-GFX-Library), © 2012 Adafruit Industries, BSD license. The full notice is in that file.

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "display_driver.h"
#include "graphic_renderer.h"

void gpio_set_level(int16_t gpio, int16_t level) {
    printf("gpio: %d, level: %d\n", gpio, level);
}

void delay_us(int16_t us) {
    printf("Sleeping for %dus...\n", us);
}

#define SCREEN_WIDTH (24)
#define SCREEN_HEIGHT (7)

int main() {
    uint8_t* image_buffer = (uint8_t*) malloc(SCREEN_HEIGHT * SCREEN_WIDTH * sizeof(uint8_t));

    graphic_renderer_config_t renderer_config = {
        .height = SCREEN_HEIGHT,
        .width = SCREEN_WIDTH,
        .image_buffer = image_buffer
    };

    graphic_renderer_data_t* renderer = graphic_renderer_init(&renderer_config);


    display_driver_config_t display_config = {
        .data_clk_pin = 1,
        .data_pin = 2,
        .refresh_clk_pin = 3,
        .display_height = SCREEN_HEIGHT,
        .display_width = SCREEN_WIDTH,
        .set_gpio = gpio_set_level,
        .delay_us = delay_us
    };

    display_driver_data_t* display = display_driver_init(&display_config);

    graphic_renderer_render_text(renderer, "hello");
    graphic_renderer_show_display(renderer);

    display_driver_scan(display, image_buffer);

    return 0;
}

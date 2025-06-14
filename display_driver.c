#include "display_driver.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "characters.h"

static display_driver_config_t driver_config;
static const uint8_t *image_buffer = NULL;
static uint8_t *internal_image_buffer = NULL;

void display_driver_init(display_driver_config_t *config) {
    if (config) {
        memcpy(&driver_config, config, sizeof(display_driver_config_t));
        uint32_t buffer_size = driver_config.display_width * driver_config.display_height;
        if (internal_image_buffer) free(internal_image_buffer);
        internal_image_buffer = malloc(buffer_size);
        if (internal_image_buffer) memset(internal_image_buffer, 0, buffer_size);
        image_buffer = internal_image_buffer;
    } else {
        image_buffer = NULL;
    }
}

void display_driver_set_image(const uint8_t *image_data) {
    image_buffer = image_data;
}

void display_driver_render_text(const char *text) {
    uint16_t char_index = 0;

    char c = text[char_index];
    uint8_t available_character = c != 0;
    uint8_t available_space = char_index * CHARACTER_WIDTH < driver_config.display_width;

    while (available_character && available_space) {
        const uint8_t* character = get_character(c);
        for (uint16_t row = 0; row < 7; row++) {
            for (uint16_t col = 0; col < CHARACTER_WIDTH; col++) {
                uint8_t bit_value = (character[row] & (1 << (8 - col))) > 0;
                internal_image_buffer[row * driver_config.display_width + char_index * CHARACTER_WIDTH + col] = bit_value ? 1 : 0;
            }
        }

        char_index++;
        c = text[char_index];
        available_character = c != 0;
        available_space = char_index * CHARACTER_WIDTH < driver_config.display_width;
    }
}

void display_driver_scan(void) {
    static uint8_t last_row = 0;
    if (!driver_config.row_output_callback || !image_buffer) return;

    uint32_t start_index = last_row * driver_config.display_width;
    uint8_t *row_data = (uint8_t *)(image_buffer + start_index);
    driver_config.row_output_callback(last_row, row_data, &driver_config);

    last_row = (last_row + 1) % driver_config.display_height;
}

void display_driver_show_display() {
    if (!image_buffer) return;

    for (uint8_t row = 0; row < driver_config.display_height; row++) {
        for (uint16_t col = 0; col < driver_config.display_width; col++) {
            uint8_t pixel = image_buffer[row * driver_config.display_width + col];
            putchar(pixel ? '#' : ' ');
            putchar(' ');
        }
        printf("\n");
    }
}

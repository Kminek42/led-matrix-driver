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
        uint32_t buffer_size = ((driver_config.display_width + 7) / 8) * driver_config.display_height;
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
    uint16_t bytes_per_row = (driver_config.display_width + 7) / 8;

    char c = text[char_index];
    uint8_t available_character = c != 0;
    uint8_t available_space = char_index < 8 * bytes_per_row / 6;

    while (available_character && available_space) {
        const uint8_t* character = get_character(c);
        for (uint16_t row = 0; row < 7; row++) {
            for (uint16_t bit = 0; bit < CHARACTER_WIDTH; bit++) {
                uint16_t bit_to_insert = 8 * bytes_per_row * row + char_index * CHARACTER_WIDTH + bit;
                uint16_t byte_to_insert = bit_to_insert / 8;
                bit_to_insert = bit_to_insert % 8;
                uint8_t bit_value = (character[row] & (1 << (8 - bit))) > 0;
                internal_image_buffer[byte_to_insert] |= (bit_value << (7 - bit_to_insert));
            }
        }

        char_index++;
        c = text[char_index];
        available_character = c != 0;
        available_space = char_index < 8 * bytes_per_row / 6;
    }
}

void display_driver_scan(display_driver_row_callback_t row_callback) {
    static uint8_t last_row = 0;
    if (!row_callback || !image_buffer) return;

    uint8_t bytes_per_row = (driver_config.display_width + 7) / 8;
    uint8_t row = last_row;

    uint8_t row_data[bytes_per_row];
    memset(row_data, 0, bytes_per_row);

    const uint8_t *src = image_buffer + row * bytes_per_row;
    memcpy(row_data, src, bytes_per_row);

    row_callback(row, row_data, &driver_config);

    last_row = (last_row + 1) % driver_config.display_height;
}

void display_driver_show_display() {
    if (!image_buffer) return;

    uint16_t bytes_per_row = (driver_config.display_width + 7) / 8;

    for (uint8_t row = 0; row < driver_config.display_height; row++) {
        for (uint16_t byte_index = 0; byte_index < bytes_per_row; byte_index++) {
            uint8_t byte = internal_image_buffer[row * bytes_per_row + byte_index];
            for (uint8_t bit_index = 0; bit_index < 8; bit_index++) {
                uint8_t pixel_index = byte_index * 8 + bit_index;
                if (pixel_index >= driver_config.display_width) break;
                char c = (byte & (1 << (7 - bit_index))) ? '#' : '_';
                putchar(c);
                putchar(' ');
            }
        }
        putchar('\n');
    }
}

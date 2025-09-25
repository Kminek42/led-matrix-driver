#include "graphic_renderer.h"
#include "characters.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

struct graphic_renderer_data {
    uint8_t width;
    uint8_t height;
    uint8_t* image_buffer;
};

graphic_renderer_data_t* graphic_renderer_init(graphic_renderer_config_t* config) {
    graphic_renderer_data_t* renderer = malloc(sizeof(graphic_renderer_data_t));

    renderer->width = config->width;
    renderer->height = config->height;
    renderer->image_buffer = config->image_buffer;

    return renderer;
}

void graphic_renderer_put_pixel(
    graphic_renderer_data_t* renderer,
    uint16_t row,
    uint16_t col,
    uint8_t value
) {
    uint16_t pixel_id = row * renderer->width + col;
    renderer->image_buffer[pixel_id] = value;
}

void graphic_renderer_render_text(graphic_renderer_data_t* renderer, const char* text) {
    uint16_t char_index = 0;

    char c = text[char_index];
    uint8_t available_character = c != 0;
    uint8_t available_space = char_index * CHARACTER_WIDTH < renderer->width;

    while (available_character && available_space) {
        const uint8_t* character = get_character(c);
        for (uint16_t row = 0; row < 7; row++) {
            for (uint16_t col = 0; col < CHARACTER_WIDTH; col++) {
                uint8_t bit_value = (character[row] & (1 << (8 - col))) > 0;
                graphic_renderer_put_pixel(
                    renderer,
                    row,
                    char_index * CHARACTER_WIDTH + col,
                    bit_value
                );
            }
        }

        char_index++;
        c = text[char_index];
        available_character = c != 0;
        available_space = char_index * CHARACTER_WIDTH < renderer->width;
    }
}

int16_t get_last_pixel_row(graphic_renderer_data_t* renderer, uint32_t column) {
    uint32_t row = 0;

    while (row < renderer->height) {
        uint32_t pixel_id = row * renderer->width + column;
        if (renderer->image_buffer[pixel_id]) {
            return row;
        }
        row += 1;
    }

    return -1;
}

void _make_plot_continous(graphic_renderer_data_t* renderer) {
    int16_t last_height = get_last_pixel_row(renderer, 0);
    for (uint32_t col = 1; col < renderer->width; col++) {
        int16_t current_height = get_last_pixel_row(renderer, col);

        int16_t difference = current_height - last_height;
        int16_t direction = (difference > 0) - (difference < 0);
        uint16_t row = last_height;

        if (abs(difference) > 1) {
            for (int32_t i = 0; i < abs(difference); i++) {
                graphic_renderer_put_pixel(renderer, row, col, 1);
                row += direction;
            }
        }

        last_height = current_height;
    }
}

void graphic_renderer_render_plot(
    graphic_renderer_data_t* renderer,
    float (*function)(float x, float* params),
    float* params,
    float x_start,
    float x_end
) {
    float f_min = 1e6f;
    float f_max = -1e6f;
    float x_axis_coef = (x_end - x_start) / ((float)(renderer->width));

    for (uint16_t col = 0; col < renderer->width; col++) {
        float x = x_start + ((float)col) * x_axis_coef;
        float f_value = function(x, params);

        if (f_value > f_max) {
            f_max = f_value;
        }
        if (f_value < f_min) {
            f_min = f_value;
        }
    }


    float y_axis_coef = 0.f;
    if (f_max - f_min > 1e-3f) {
        y_axis_coef = 1.f / (f_max - f_min);
    }

    graphic_renderer_clear(renderer);
    for (uint16_t col = 0; col < renderer->width; col++) {
        float x = x_start + ((float)col) * x_axis_coef;
        float f_value = (function(x, params) - f_min) * y_axis_coef;
        uint16_t f_value_row = (uint16_t)(f_value * ((float)(renderer->height - 1) + 0.5f));
        f_value_row = renderer->height - f_value_row - 1;

        graphic_renderer_put_pixel(renderer, f_value_row, col, 1);
    }

    _make_plot_continous(renderer);
}

void graphic_renderer_clear(graphic_renderer_data_t* renderer) {
    uint32_t pixels_to_clear = renderer->height * renderer->width * sizeof(uint8_t);
    memset(renderer->image_buffer, 0, pixels_to_clear);
}

void graphic_renderer_show_display(graphic_renderer_data_t* renderer) {
    for (uint8_t row = 0; row < renderer->height; row++) {
        for (uint16_t col = 0; col < renderer->width; col++) {
            uint8_t pixel = renderer->image_buffer[row * renderer->width + col];
            putchar(pixel ? '#' : ' ');
            putchar(' ');
        }
        printf("\n");
    }
}

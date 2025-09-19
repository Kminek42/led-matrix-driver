#include "graphic_renderer.h"
#include "characters.h"
#include <stdlib.h>
#include <stdio.h>

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

void graphic_renderer_render_text(graphic_renderer_data_t* renderer, char* text) {
    uint16_t char_index = 0;

    char c = text[char_index];
    uint8_t available_character = c != 0;
    uint8_t available_space = char_index * CHARACTER_WIDTH < renderer->width;

    while (available_character && available_space) {
        const uint8_t* character = get_character(c);
        for (uint16_t row = 0; row < 7; row++) {
            for (uint16_t col = 0; col < CHARACTER_WIDTH; col++) {
                uint8_t bit_value = (character[row] & (1 << (8 - col))) > 0;
                renderer->image_buffer[row * renderer->width + char_index * CHARACTER_WIDTH + col] = bit_value ? 1 : 0;
            }
        }

        char_index++;
        c = text[char_index];
        available_character = c != 0;
        available_space = char_index * CHARACTER_WIDTH < renderer->width;
    }
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

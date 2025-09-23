#pragma once
#include <stdint.h>

typedef struct graphic_renderer_config {
    uint16_t width;
    uint16_t height;
    uint8_t* image_buffer;
} graphic_renderer_config_t;

typedef struct graphic_renderer_data graphic_renderer_data_t;

graphic_renderer_data_t* graphic_renderer_init(graphic_renderer_config_t* config);

void graphic_renderer_render_text(graphic_renderer_data_t* renderer, char* text);


void graphic_renderer_render_plot(
    graphic_renderer_data_t* renderer,
    float (*function)(float x, float* params),
    float* params,
    float x_start,
    float x_end
);

void graphic_renderer_show_display(graphic_renderer_data_t* renderer);

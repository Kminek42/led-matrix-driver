#pragma once
#include <stdint.h>

typedef struct display_driver_config display_driver_config_t;

typedef void (*row_output_callback_t)(uint16_t row_n, uint8_t* row_data, display_driver_config_t *config);

struct display_driver_config {
    int display_width;
    int display_height;
    row_output_callback_t row_output_callback;
};

void display_driver_init(display_driver_config_t *config);
void display_driver_scan(void);
void display_driver_set_image(const uint8_t *image_data);
void display_driver_render_text(const char *text);
void display_driver_show_display();

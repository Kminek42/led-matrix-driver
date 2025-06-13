#pragma once
#include <stdint.h>

typedef struct {
    uint16_t display_width;
    uint16_t display_height;
    uint16_t ser_pin;
    uint16_t sclk_pin;
    uint16_t rclk_pin;
} display_driver_config_t;

typedef void (*display_driver_row_callback_t)(uint8_t row, uint8_t *row_data, const display_driver_config_t *config);

void display_driver_init(display_driver_config_t *config);

void display_driver_scan(display_driver_row_callback_t row_callback);

void display_driver_set_image(const uint8_t *image_data);

void display_driver_render_text(const char *text);

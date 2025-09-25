#pragma once
#include <stdint.h>

typedef struct {
    int16_t display_width;
    int16_t display_height;
    int16_t data_pin;
    int16_t data_clk_pin;
    int16_t refresh_clk_pin;
    void (*set_gpio)(int16_t pin, int16_t value);
    void (*delay_us)(int16_t us);
} display_driver_config_t;

typedef struct display_driver_data display_driver_data_t;

display_driver_data_t* display_driver_init(display_driver_config_t* config);
void display_driver_scan(
    display_driver_data_t* display, 
    uint8_t* displayed_image_buffer, 
    uint8_t* staging_image_buffer
);

#include "display_driver.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "characters.h"

struct display_driver_data {
    display_driver_config_t* config;
    uint16_t last_row;
    uint8_t buffor_busy;
};

display_driver_data_t* display_driver_init(display_driver_config_t* config) {
    display_driver_data_t* driver_data = malloc(sizeof(display_driver_data_t));
    driver_data->config = config;
    driver_data->last_row = 0;
    driver_data->buffor_busy = 0;

    return driver_data;
}

void display_clear(display_driver_config_t* config, uint8_t row_id) {
    uint32_t total_bits = config->display_height + config->display_width;
    uint32_t total_bytes = (total_bits + 7) / 8;
    uint32_t unused_bits = (8 * total_bytes) - total_bits;

    for (int i = 0; i < config->display_width; i++) {
        config->set_gpio(config->data_pin, 1);
        config->delay_us(1);
        config->set_gpio(config->data_clk_pin, 1);
        config->delay_us(1);
        config->set_gpio(config->data_clk_pin, 0);
        config->delay_us(1);
    }

    for (int i = 0; i < config->display_height + unused_bits; i++) {
        uint8_t bit = (config->display_height - row_id - 1) == i;
        config->set_gpio(config->data_pin, bit);
        config->delay_us(1);
        config->set_gpio(config->data_clk_pin, 1);
        config->delay_us(1);
        config->set_gpio(config->data_clk_pin, 0);
        config->delay_us(1);
    }

    config->set_gpio(config->refresh_clk_pin, 1);
    config->delay_us(1);
    config->set_gpio(config->refresh_clk_pin, 0);
    config->delay_us(10);
}

void display_send_row(uint16_t row_n, uint8_t* row_data, display_driver_config_t* config) {
    int16_t previous_row_id = row_n - 1;
    if (previous_row_id < 0) {
        previous_row_id += config->display_height;
    }

    display_clear(config, previous_row_id);

    uint32_t total_bits = config->display_height + config->display_width;
    uint32_t total_bytes = (total_bits + 7) / 8;
    uint32_t unused_bits = (8 * total_bytes) - total_bits;

    for (int i = 0; i < config->display_width; i++) {
        config->set_gpio(config->data_pin, !row_data[i]);
        config->delay_us(1);
        config->set_gpio(config->data_clk_pin, 1);
        config->delay_us(1);
        config->set_gpio(config->data_clk_pin, 0);
        config->delay_us(1);
    }

    for (int i = 0; i < config->display_height; i++) {
        uint8_t bit = (config->display_height - row_n - 1) == i;
        config->set_gpio(config->data_pin, bit);
        config->delay_us(1);
        config->set_gpio(config->data_clk_pin, 1);
        config->delay_us(1);
        config->set_gpio(config->data_clk_pin, 0);
        config->delay_us(1);
    }

    for (int i = 0; i < unused_bits; i++) {
        config->set_gpio(config->data_clk_pin, 1);
        config->delay_us(1);
        config->set_gpio(config->data_clk_pin, 0);
        config->delay_us(1);
    }


    config->set_gpio(config->refresh_clk_pin, 1);
    config->delay_us(1);
    config->set_gpio(config->refresh_clk_pin, 0);
    config->delay_us(1);
}

void display_driver_take_buffor(display_driver_data_t* display) {
    display->buffor_busy = 1;
}

void display_driver_release_buffor(display_driver_data_t* display) {
    display->buffor_busy = 0;
}

void display_driver_scan(
    display_driver_data_t* display,
    uint8_t* displayed_image_buffer,
    uint8_t* staging_image_buffer
) {
    uint32_t start_index = display->last_row * display->config->display_width;
    uint8_t* row_data = (uint8_t*)(displayed_image_buffer + start_index);
    display_send_row(display->last_row, row_data, display->config);

    display->last_row = (display->last_row + 1) % display->config->display_height;

    uint32_t image_size = display->config->display_width * display->config->display_height;
    if (display->last_row == 0 && !display->buffor_busy) {
        memcpy(displayed_image_buffer, staging_image_buffer, image_size);
    }
}

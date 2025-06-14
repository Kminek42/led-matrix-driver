#include <stdio.h>
#include "display_driver.h"

int main() {
    display_driver_config_t config = {
        .display_width = 24,
        .display_height = 7,
        .ser_pin = 2,
        .sclk_pin = 3,
        .rclk_pin = 4
    };
    display_driver_init(&config);
    display_driver_render_text("Osc1");
    display_driver_show_display();
    return 0;
}
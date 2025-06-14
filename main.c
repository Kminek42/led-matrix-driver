#include <stdio.h>
#include "display_driver.h"

void my_row_output_callback(uint16_t row_n, uint8_t* row_data, display_driver_config_t *config) {
    printf("Row %d: ", row_n);
    for (int i = 0; i < (config->display_width + 7) / 8; i++) {
        for (uint8_t bit = 0; bit < 8; bit++) {
            if (row_data[i] & (1 << (7 - bit))) {
                printf("o ");
            } else {
                printf("  ");
            }
        }
    }
    printf("\n");
}

int main() {
    display_driver_config_t config = {
        .display_width = 24,
        .display_height = 7,
        .row_output_callback = my_row_output_callback,
    };
    display_driver_init(&config);
    display_driver_render_text("Osc1");
    // display_driver_show_display();
    display_driver_scan();
    display_driver_scan();
    display_driver_scan();
    display_driver_scan();
    display_driver_scan();
    display_driver_scan();
    display_driver_scan();
    return 0;
}
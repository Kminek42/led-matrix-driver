#include <stdio.h>
#include "display_driver.h"

void my_row_output_callback(uint16_t row_n, uint8_t* row_data, display_driver_config_t *config) {
    printf("Row %d: ", row_n);
    uint16_t indent = config->display_height - row_n;
    while(indent) {
        putchar(' ');
        indent -= 1;
    }
    for (int col = 0; col < config->display_width; col++) {
        uint8_t pixel = row_data[col];
        putchar(pixel ? '#' : '.');
        putchar(' ');
    }
    printf("\n");
}

int main() {
    display_driver_config_t config = {
        .display_width = 64,
        .display_height = 8,
        .row_output_callback = my_row_output_callback,
    };
    display_driver_init(&config);
    display_driver_render_text("Kminek42");
    display_driver_show_display();
    display_driver_scan();
    display_driver_scan();
    display_driver_scan();
    display_driver_scan();
    display_driver_scan();
    display_driver_scan();
    display_driver_scan();
    display_driver_scan();
    return 0;
}
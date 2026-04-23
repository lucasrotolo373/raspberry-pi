#include <gpiod.h>
#include <stdio.h>
#include <unistd.h>

int main() {
    struct gpiod_chip *chip;
    struct gpiod_line_request *request;
    struct gpiod_request_config *req_cfg;
    struct gpiod_line_config *line_cfg;
    struct gpiod_line_settings *settings;

    chip = gpiod_chip_open("/dev/gpiochip0");
    if (!chip) {
        perror("gpiod_chip_open");
        return 1;
    }

    settings = gpiod_line_settings_new();
    gpiod_line_settings_set_direction(settings, GPIOD_LINE_DIRECTION_OUTPUT);

    line_cfg = gpiod_line_config_new();
    gpiod_line_config_add_line_settings(line_cfg, (unsigned int[]){17}, 1, settings);

    req_cfg = gpiod_request_config_new();
    gpiod_request_config_set_consumer(req_cfg, "blink");

    request = gpiod_chip_request_lines(chip, req_cfg, line_cfg);

    for (int i = 0; i < 10; i++) {
        gpiod_line_request_set_value(request, 17, GPIOD_LINE_VALUE_ACTIVE);
        sleep(1);
        gpiod_line_request_set_value(request, 17, GPIOD_LINE_VALUE_INACTIVE);
        sleep(1);
    }

    gpiod_line_request_release(request);
    gpiod_request_config_free(req_cfg);
    gpiod_line_config_free(line_cfg);
    gpiod_line_settings_free(settings);
    gpiod_chip_close(chip);
    return 0;
}

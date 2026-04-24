#include <gpiod.h>
#include <stdio.h>
#include <unistd.h>

int estado_actual = GPIOD_LINE_VALUE_INACTIVE;
int estado_anterior = GPIOD_LINE_VALUE_INACTIVE;

int main() {
    struct gpiod_chip *chip;
    struct gpiod_line_request *req_led;
    struct gpiod_line_request *req_btn;
    struct gpiod_line_config *cfg_led;
    struct gpiod_line_config *cfg_btn;
    struct gpiod_line_settings *set_led;
    struct gpiod_line_settings *set_btn;
    struct gpiod_request_config *rc_led;
    struct gpiod_request_config *rc_btn;
    
    chip = gpiod_chip_open("/dev/gpiochip0");
    if (!chip) {
        perror("gpiod_chip_open");
        return 1;
    }

    // Configurar LED como salida
    set_led = gpiod_line_settings_new();
    gpiod_line_settings_set_direction(set_led, GPIOD_LINE_DIRECTION_OUTPUT);
    cfg_led = gpiod_line_config_new();
    gpiod_line_config_add_line_settings(cfg_led, (unsigned int[]){17}, 1, set_led);
    rc_led = gpiod_request_config_new();
    gpiod_request_config_set_consumer(rc_led, "led");
    req_led = gpiod_chip_request_lines(chip, rc_led, cfg_led);

    // Configurar pulsador como entrada con pull-down
    set_btn = gpiod_line_settings_new();
    gpiod_line_settings_set_direction(set_btn, GPIOD_LINE_DIRECTION_INPUT);
    gpiod_line_settings_set_bias(set_btn, GPIOD_LINE_BIAS_PULL_DOWN);
    cfg_btn = gpiod_line_config_new();
    gpiod_line_config_add_line_settings(cfg_btn, (unsigned int[]){27}, 1, set_btn);
    rc_btn = gpiod_request_config_new();
    gpiod_request_config_set_consumer(rc_btn, "boton");
    req_btn = gpiod_chip_request_lines(chip, rc_btn, cfg_btn);

    // Loop principal
    while (1) {
    	enum gpiod_line_value lectura = gpiod_line_request_get_value(req_btn, 27);

    	if (lectura != estado_anterior) {
        	usleep(30000); // esperamos 30ms
        	lectura = gpiod_line_request_get_value(req_btn, 27); // leemos de nuevo
        	if (lectura != estado_actual) {
            		estado_actual = lectura;
            		gpiod_line_request_set_value(req_led, 17, estado_actual);
        	}
    	}
    	estado_anterior = lectura;
    	usleep(1000);
    }
    return 0;
}

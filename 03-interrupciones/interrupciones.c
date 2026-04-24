#include <gpiod.h>
#include <stdio.h>
#include <unistd.h>

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
    struct gpiod_edge_event_buffer *buffer;
    struct gpiod_edge_event *event;

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

    // Configurar pulsador como entrada con eventos de flanco
    set_btn = gpiod_line_settings_new();
    gpiod_line_settings_set_direction(set_btn, GPIOD_LINE_DIRECTION_INPUT);
    gpiod_line_settings_set_bias(set_btn, GPIOD_LINE_BIAS_PULL_DOWN);
    gpiod_line_settings_set_edge_detection(set_btn, GPIOD_LINE_EDGE_BOTH);
    cfg_btn = gpiod_line_config_new();
    gpiod_line_config_add_line_settings(cfg_btn, (unsigned int[]){27}, 1, set_btn);
    rc_btn = gpiod_request_config_new();
    gpiod_request_config_set_consumer(rc_btn, "boton");
    req_btn = gpiod_chip_request_lines(chip, rc_btn, cfg_btn);

    buffer = gpiod_edge_event_buffer_new(1);

    int led_state = 0;

    printf("Esperando eventos...\n");

    while (1) {
        // Bloquea hasta que ocurre un evento
        gpiod_line_request_wait_edge_events(req_btn, -1);
        gpiod_line_request_read_edge_events(req_btn, buffer, 1);

        event = gpiod_edge_event_buffer_get_event(buffer, 0);

        if (gpiod_edge_event_get_event_type(event) == GPIOD_EDGE_EVENT_RISING_EDGE) {
            led_state = 1;
            printf("Pulsador presionado\n");
        } else {
            led_state = 0;
            printf("Pulsador suelto\n");
        }

        gpiod_line_request_set_value(req_led, 17, led_state);
    }

    return 0;
}

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>
#include <syslog.h>
#include <modbus/modbus.h>

#define ESP32_IP    "192.168.1.29"
#define ESP32_PORT   502
#define REG_COUNTER  0
#define REG_LED      2
#define RETRY_DELAY  5

static volatile int running = 1;

void handle_signal(int sig) {
    syslog(LOG_INFO, "Señal recibida %d, cerrando...", sig);
    running = 0;
}

int main() {
    openlog("modbus-service", LOG_PID, LOG_DAEMON);
    syslog(LOG_INFO, "Servicio iniciado");

    signal(SIGTERM, handle_signal);
    signal(SIGINT, handle_signal);

    while (running) {
        modbus_t *ctx = modbus_new_tcp(ESP32_IP, ESP32_PORT);
        if (ctx == NULL) {
            syslog(LOG_ERR, "Error creando contexto Modbus");
            sleep(RETRY_DELAY);
            continue;
        }

        if (modbus_connect(ctx) == -1) {
            syslog(LOG_ERR, "Error conectando al ESP32: %s", modbus_strerror(errno));
            modbus_free(ctx);
            sleep(RETRY_DELAY);
            continue;
        }

        syslog(LOG_INFO, "Conectado al ESP32");

        uint16_t regs[1];
        int rc;
        int led = 0;

        while (running) {
            rc = modbus_read_registers(ctx, REG_COUNTER, 1, regs);
            if (rc == -1) {
                syslog(LOG_ERR, "Error leyendo registro: %s", modbus_strerror(errno));
                break;
            }
            syslog(LOG_INFO, "Contador: %d", regs[0]);

            led = !led;
            rc = modbus_write_register(ctx, REG_LED, led);
            if (rc == -1) {
                syslog(LOG_ERR, "Error escribiendo registro: %s", modbus_strerror(errno));
                break;
            }

            sleep(1);
        }

        modbus_close(ctx);
        modbus_free(ctx);

        if (running) {
            syslog(LOG_WARNING, "Conexion perdida, reintentando en %d segundos", RETRY_DELAY);
            sleep(RETRY_DELAY);
        }
    }

    syslog(LOG_INFO, "Servicio detenido");
    closelog();
    return 0;
}

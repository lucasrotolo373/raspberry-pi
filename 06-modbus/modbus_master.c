#include <stdio.h>
#include <unistd.h>
#include <modbus/modbus.h>
#include <errno.h>

#define ESP32_IP   "192.168.1.29"
#define ESP32_PORT  502
#define REG_COUNTER 0
#define REG_LED     2

int main() {
    modbus_t *ctx;
    uint16_t regs[3];
    int rc;

    ctx = modbus_new_tcp(ESP32_IP, ESP32_PORT);
    if (ctx == NULL) {
        fprintf(stderr, "Error creando contexto Modbus\n");
        return 1;
    }

    if (modbus_connect(ctx) == -1) {
        fprintf(stderr, "Error conectando: %s\n", modbus_strerror(errno));
        modbus_free(ctx);
        return 1;
    }

    printf("Conectado al ESP32\n");

    for (int i = 0; i < 10; i++) {
        rc = modbus_read_registers(ctx, REG_COUNTER, 1, regs);
        if (rc == -1) {
            fprintf(stderr, "Error leyendo: %s\n", modbus_strerror(errno));
            break;
        }
        printf("Contador ESP32: %d\n", regs[0]);

        uint16_t led = i % 2;
        rc = modbus_write_register(ctx, REG_LED, led);
        if (rc == -1) {
            fprintf(stderr, "Error escribiendo: %s\n", modbus_strerror(errno));
            break;
        }
        printf("LED -> %d\n", led);

        sleep(1);
    }

    modbus_close(ctx);
    modbus_free(ctx);
    return 0;
}

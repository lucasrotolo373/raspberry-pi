# 05 - UART — Comunicación Pi con ESP32

Comunicación serial entre Raspberry Pi 3B+ y ESP32 usando UART desde C puro.

## Concepto
El puerto serial se accede como archivo en /dev/serial0. La configuración
del puerto (velocidad, bits, paridad) se hace con la estructura termios.
El modo raw desactiva el procesamiento del kernel — lo que escribís es
exactamente lo que sale por el cable byte a byte.

## Hardware
- Raspberry Pi 3B+
- ESP32 conectado por UART2 (GPIO16 RX, GPIO17 TX)
- Pi TX (pin 8) → ESP32 GPIO16
- Pi RX (pin 10) → ESP32 GPIO17
- GND compartido

## Configuración
- Baud rate: 115200
- Frame: 8N1 (8 bits, sin paridad, 1 stop bit)
- Modo raw

## Compilar y ejecutar
make
./uart_send

## Código ESP32 (receptor)

El ESP32 actúa como bridge — recibe por Serial2 (GPIO16/17) y reenvía
por Serial0 al monitor serial de la Mac.

```cpp
#include <Arduino.h>

void setup() {
  Serial.begin(115200);
  Serial2.begin(115200, SERIAL_8N1, 16, 17);
  Serial.println("ESP32 listo, esperando datos de la Pi...");
}

void loop() {
  if (Serial2.available()) {
    String msg = Serial2.readStringUntil('\n');
    Serial.println("Pi dice: " + msg);
  }
}
```

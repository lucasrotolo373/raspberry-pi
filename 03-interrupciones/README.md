# 03 - Interrupciones (Edge Events)

Detección de eventos de flanco en GPIO usando libgpiod v2. 
El programa bloquea hasta que ocurre un cambio en el pin — sin polling.

## Concepto
En vez de preguntar continuamente el estado del pin (polling), el programa
se bloquea en wait_edge_events() y el kernel lo despierta cuando detecta
un cambio. La CPU queda libre hasta que ocurre el evento.

## Diferencia con ESP32
En ESP32 la interrupción salta directamente al handler a nivel de hardware.
En Linux userspace no hay acceso directo al hardware — el kernel recibe la
interrupción, la procesa, y despierta el proceso bloqueado.

## Hardware
- Raspberry Pi 3B+
- LED en GPIO17 (pin físico 11) con resistencia 220Ω
- Pulsador entre 3.3V (pin 1) y GPIO27 (pin físico 13)

## Compilar y ejecutar
make
./interrupciones

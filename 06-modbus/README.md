# 06 - Modbus TCP

Cliente Modbus TCP en C puro corriendo en la Raspberry Pi 3B+.
Se conecta a un ESP32 actuando como servidor Modbus y lee/escribe registros.

## Qué hace
- Lee el registro 0 (contador que incrementa cada segundo en el ESP32)
- Escribe el registro 2 (controla el LED integrado del ESP32)
- 10 iteraciones con 1 segundo de intervalo

## Por qué Modbus
Modbus es el protocolo industrial más usado del mundo. Ningún dispositivo
conoce la implementación interna del otro — solo conocen el mapa de registros.
El ESP32 no sabe que hay una Pi. La Pi no sabe cómo funciona el ESP32.
Ambos interactúan a través de una tabla compartida de registros de 16 bits.

## Protocolo
- Modbus TCP sobre WiFi
- Puerto 502 (estándar Modbus)
- Function code FC03 para lectura de Holding Registers
- Function code FC06 para escritura de un Holding Register

## Dependencias
- libmodbus: sudo apt install libmodbus-dev

## Compilar y ejecutar
make
./modbus_master

## Hardware
- Raspberry Pi 3B+ (maestro Modbus)
- ESP32 NodeMCU 38-pin (esclavo Modbus, firmware en repo curso-microcontroladores)

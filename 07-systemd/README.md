# 07 - systemd service

Cliente Modbus TCP convertido en servicio de Linux con systemd.

## Qué hace
- Corre indefinidamente leyendo el contador del ESP32 y alternando su LED
- Se reconecta automáticamente si pierde la conexión
- Maneja SIGTERM y SIGINT para terminar limpiamente
- Loguea eventos con syslog en lugar de printf

## Conceptos
- systemd units y archivos .service
- Manejo de señales con signal()
- syslog para logging en servicios
- Resiliencia: reconexión automática ante pérdida de conexión

## Comandos útiles
sudo systemctl start modbus      # arrancar
sudo systemctl stop modbus       # parar
sudo systemctl status modbus     # ver estado y logs recientes
journalctl -t modbus-service     # ver historial completo de logs
journalctl -t modbus-service -n 20  # ver últimas 20 líneas

## Dependencias
- libmodbus: sudo apt install libmodbus-dev

## Compilar
make

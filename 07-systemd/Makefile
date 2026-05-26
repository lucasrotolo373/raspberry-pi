CC = gcc
CFLAGS = -Wall
LIBS = -lmodbus

all: modbus_service

modbus_service: modbus_service.c
	$(CC) $(CFLAGS) modbus_service.c -o modbus_service $(LIBS)

clean:
	rm -f modbus_service

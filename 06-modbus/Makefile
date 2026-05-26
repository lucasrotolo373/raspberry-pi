CC = gcc
CFLAGS = -Wall
LIBS = -lmodbus

all: modbus_master

modbus_master: modbus_master.c
	$(CC) $(CFLAGS) modbus_master.c -o modbus_master $(LIBS)

clean:
	rm -f modbus_master

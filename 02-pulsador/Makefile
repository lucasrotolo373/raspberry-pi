CC = gcc
CFLAGS = -Wall
LIBS = -lgpiod

all: pulsador

pulsador: pulsador.c
	$(CC) $(CFLAGS) pulsador.c -o pulsador $(LIBS)

clean:
	rm -f pulsador

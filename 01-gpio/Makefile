CC = gcc
CFLAGS = -Wall
LIBS = -lgpiod

all: blink

blink: blink.c
	$(CC) $(CFLAGS) blink.c -o blink $(LIBS)

clean:
	rm -f blink

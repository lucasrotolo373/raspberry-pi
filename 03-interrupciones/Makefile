CC = gcc
CFLAGS = -Wall
LIBS = -lgpiod

all: interrupciones

interrupciones: interrupciones.c
	$(CC) $(CFLAGS) interrupciones.c -o interrupciones $(LIBS)

clean:
	rm -f interrupciones

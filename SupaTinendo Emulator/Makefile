# ⚡ SUPATINENDO MAKEFILE ⚡

CC = gcc
CFLAGS = -O3 $(shell sdl2-config --cflags)
LIBS = $(shell sdl2-config --libs)
TARGET = SupaTinendo

all:
	$(CC) $(CFLAGS) main.c -o $(TARGET) $(LIBS)

clean:
	rm -f $(TARGET)
	rm -f ST_memory_dump.txt

CC=gcc
CFLAGS= -g -ggdb -Wall -Wextra -std=gnu99 -D DEBUG_MODE
LDFLAGS= -L/usr/local/lib
LDLIBS= -lSDL2main -lSDL2
kozoume: kozoume.o interpreter.o io.o ppu.o memory.o z80.o cartridge.o opcodes.o interrupts.o console.o render.o audio.o

clean:
	rm -f kozoume kozoume.exe kozoume.o interpreter.o io.o ppu.o memory.o z80.o cartridge.o opcodes.o interrupts.o console.o render.o audio.o

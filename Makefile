CC = gcc
C_COMPAT    = -Wpedantic -std=c99
LIB_FLAGS   = -lSDL2
WARNINGS    = -Wall -Werror
DEBUG_FLAGS = -g
CFLAGS =  $(C_COMPAT) $(LIB_FLAGS) $(WARNINGS) $(DEBUG_FLAGS)

chip8: chip8.o memory.o cpu.o graphics.o
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm -rf *.o chip8

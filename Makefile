CC = gcc
CCFLAGS = -Wall -Werror -lSDL2 -g

chip8: chip8.o memory.o cpu.o graphics.o
	$(CC) $(CCFLAGS) -o $@ $^

memory.o: memory.c

cpu.o: cpu.c

graphics.o: graphics.c

clean:
	rm -rf *.o chip8

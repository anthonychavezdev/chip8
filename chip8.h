#ifndef CHIP8_H
#define CHIP8_H

#define MAX_ROM_SIZE (0x1000 - 0x200)

int chip8_load_rom(char *path);

#endif

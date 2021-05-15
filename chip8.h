#ifndef CHIP8_H
#define CHIP8_H
#include "memory.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <unistd.h>
#include "graphics.h"
#include "cpu.h"

#define MAX_ROM_SIZE (0x1000 - 0x200)

int chip8_load_rom(char *path);

#endif

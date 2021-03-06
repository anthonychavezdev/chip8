#ifndef MEMORY_H
#define MEMORY_H

#include <stdint.h>

#define MEM_SIZE 0x1000 // Chip8's memory is 4096 bytes.
#define STACK_SIZE 48

extern uint8_t memory[MEM_SIZE];
extern uint16_t stack[48];

void chip8_init_memory();
void chip8_init_stack();
void chip8_dump_memory(uint8_t *memory);

#endif

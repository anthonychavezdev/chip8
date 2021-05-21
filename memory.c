#include "memory.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h> // for memset

uint8_t memory[MEM_SIZE];
uint16_t stack[STACK_SIZE];

void chip8_init_memory() { memset(memory, 0x0, 1 * sizeof(memory)); }
void chip8_init_stack() { memset(stack, 0x0, 1 * sizeof(stack)); }

void chip8_dump_memory(uint8_t *memory) {
  for (size_t i = 0; i < MEM_SIZE; i++) {
    switch (i % 16) {
    case 0: {
      printf("0x%03x", (uint16_t)i);
      printf(": ");
      printf("%02x", (uint8_t)memory[i]);
    } break;
    case 8: {
      printf("  %02x", (uint8_t)memory[i]);
    } break;
    case 15: {
      printf(" %02x\n", (uint8_t)memory[i]);
    } break;
    default: {
      printf(" %02x", (uint8_t)memory[i]);
    } break;
    }
  }
}

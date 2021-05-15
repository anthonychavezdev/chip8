#ifndef REGISTERS_H
#define REGISTERS_H

#include <stdint.h>
#include <string.h> // for memset

#define NUM_OF_GPRS  16 // How many general purpose registers there are.

struct regs
{
    uint8_t V[NUM_OF_GPRS]; // 16 general purpose registers. 0x0-0xF
    uint8_t dt;             // delay timer register
    uint8_t st;             // sound timer register
    uint16_t I;             // holds memory addresses. (rightmost 12 bits)
    uint16_t pc;            // Program counter. Not accessible to programs
    uint16_t sp;            // holds 16 values. Points to the topmost item
};
extern struct regs regs;

void chip8_init_regs();
void advance_pc(uint16_t pc, uint16_t addr);
#endif

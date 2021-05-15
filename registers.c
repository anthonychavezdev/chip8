#include "registers.h"
#include <stdio.h>

struct regs regs;

void chip8_init_regs() {
  regs.pc = 0x200;
  regs.sp = 48; // Starting at the top of the stack, and growing down
  regs.I = 0;
  regs.dt = regs.st = 0;
  memset(regs.V, 0x00, sizeof(uint8_t) * NUM_OF_GPRS);

  printf("init_regs pc: 0x%x\n", regs.pc);
}
void advance_pc(uint16_t pc, uint16_t addr) {}

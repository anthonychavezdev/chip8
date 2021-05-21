#include "cpu.h"
#include "graphics.h"
#include "input.h"
#include "memory.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h> // for memset
#include <time.h>

struct cpu cpu;

void chip8_init_regs() {
  cpu.pc = 0x200;
  cpu.sp = 0; // Starting at the top of the stack, and growing down
  cpu.I = 0;
  cpu.dt = cpu.st = 0;
  memset(cpu.V, 0, sizeof(uint8_t) * NUM_OF_GPRS);

  /* printf("init_regs pc: 0x%x\n", cpu.pc); */
}
void chip8_update_timers() {
  if (cpu.dt > 0)
    --cpu.dt;
  if (cpu.st > 0)
    --cpu.st;
}
uint16_t chip8_fetch_opcode(uint16_t addr) {
  return memory[addr] << 8 | memory[addr + 1];
}

void chip8_execute_00E0() {
  chip8_clear_screen();
  cpu.pc += 2;
}

void chip8_execute_00EE() { cpu.pc = stack[--cpu.sp]; }

void chip8_execute_1nnn(uint16_t addr) { cpu.pc = addr; }

void chip8_execute_2nnn(uint16_t addr) {
  stack[cpu.sp++] = cpu.pc + 2;
  cpu.pc = addr;
}
void chip8_execute_3xkk(int reg, int kk) {
  cpu.pc += (cpu.V[reg] == kk) ? 4 : 2;
}

void chip8_execute_4xkk(int reg, int kk) {
  cpu.pc += (cpu.V[reg] != kk) ? 4 : 2;
}

void chip8_execute_5xy0(int reg1, int reg2) {
  cpu.pc += (cpu.V[reg1] == cpu.V[reg2]) ? 4 : 2;
}

void chip8_execute_6xkk(int reg, int kk) {
  cpu.V[reg] = kk;
  cpu.pc += 2;
}

void chip8_execute_7xkk(int reg, int kk) {
  cpu.V[reg] += kk;
  cpu.pc += 2;
}

void chip8_execute_8xy0(int reg1, int reg2) {
  cpu.V[reg1] = cpu.V[reg2];
  cpu.pc += 2;
}

void chip8_execute_8xy1(int reg1, int reg2) {
  cpu.V[reg1] |= cpu.V[reg2];
  cpu.pc += 2;
}

void chip8_execute_8xy2(int reg1, int reg2) {
  cpu.V[reg1] &= cpu.V[reg2];
  cpu.pc += 2;
}

void chip8_execute_8xy3(int reg1, int reg2) {
  cpu.V[reg1] ^= cpu.V[reg2];
  cpu.pc += 2;
}

void chip8_execute_8xy4(int reg1, int reg2) {
  cpu.V[0xF] = (cpu.V[reg1] + cpu.V[reg2]) > 255 ? 1 : 0;
  cpu.V[reg1] += cpu.V[reg2];
  cpu.pc += 2;
}

void chip8_execute_8xy5(int reg1, int reg2) {
  cpu.V[0xf] = (cpu.V[reg1] > cpu.V[reg2]) ? 1 : 0;
  cpu.V[reg1] -= cpu.V[reg2];
  cpu.pc += 2;
}

void chip8_execute_8xy6(int reg1, int reg2) {
  (void)reg2;
  cpu.V[0xf] = (cpu.V[reg1] & 0x01);
  cpu.V[reg1] >>= 1;
  cpu.pc += 2;
}

void chip8_execute_8xy7(int reg1, int reg2) {
  cpu.V[0xf] = (cpu.V[reg2] > cpu.V[reg1]) ? 1 : 0;
  cpu.V[reg2] -= cpu.V[reg1];
  cpu.pc += 2;
}

void chip8_execute_8xye(int reg1, int reg2) {
  (void)reg2;
  cpu.V[0xf] = (cpu.V[reg1] & 0x80) >> 7;
  cpu.V[reg1] <<= 1;
  cpu.pc += 2;
}

void chip8_execute_9xy0(int reg1, int reg2) {
  cpu.pc += (cpu.V[reg1] != cpu.V[reg2]) ? 4 : 2;
}

void chip8_execute_Annn(uint16_t addr) {
  cpu.I = addr;
  cpu.pc += 2;
}

void chip8_execute_Bnnn(uint16_t addr) { cpu.pc = (addr + cpu.V[0]); }

void chip8_execute_Cxkk(int reg, int kk) {
  srand(time(NULL));
  uint8_t rand_num = rand() % 255;
  cpu.V[reg] = (rand_num & kk);
  cpu.pc += 2;
}

void chip8_execute_Dxyn(int reg1, int reg2, int n) {
  uint8_t pixel;
  uint8_t x = cpu.V[reg1] % 64;
  uint8_t y = cpu.V[reg2] % 32;
  uint8_t height = n;

  cpu.V[0xF] = 0;

  for (int row = 0; row < height; row++) {
    pixel = memory[cpu.I + row];

    for (int col = 0; col < 8; col++) {
      if ((pixel & (0x80 >> col)) != 0) {
        if (display[(x + col + ((y + row) * 64))] > 0) {
          cpu.V[0xF] = 1;
        }
        display[x + col + ((y + row) * 64)] ^= 1;
      } else {
        display[x + col + ((y + row) * 64)] = 0;
      }
    }
  }
  chip8_draw_screen();
  cpu.pc += 2;
}

void chip8_execute_Ex9e(int reg) {
  int index = cpu.V[reg];
  cpu.pc += (keys[index] == true) ? 4 : 2;
}

void chip8_execute_Exa1(int reg) {
  int index = cpu.V[reg];
  cpu.pc += (keys[index] == false) ? 4 : 2;
}

void chip8_execute_Fx07(int reg) {
  cpu.V[reg] = cpu.dt;
  cpu.pc += 2;
}

void chip8_execute_Fx0a(int reg) {
  for (int i = 0; i < NUM_KEYS; i++)
    if (keys[i] == true)
      cpu.V[reg] = keys[i];

  cpu.pc += 2;
}

void chip8_execute_Fx15(int reg) {
  cpu.dt = cpu.V[reg];
  cpu.pc += 2;
}

void chip8_execute_Fx18(int reg) {
  cpu.st = cpu.V[reg];
  cpu.pc += 2;
}

void chip8_execute_Fx1e(int reg) {
  cpu.I += cpu.V[reg];
  cpu.pc += 2;
}

void chip8_execute_Fx29(int reg) {
  cpu.I = FONTSET_BYTES_PER_CHAR * cpu.V[reg];
  cpu.pc += 2;
}

void chip8_execute_Fx33(int reg) {
  uint8_t num = cpu.V[reg]; // gets the number from the register
  uint8_t digits[3];        // buffer in which we will store the reversed number
  uint8_t counter = 0;
  while (num > 0) {
    uint8_t mod = num % 10; // grab last digit
    digits[counter] = mod;
    num /= 10; // Throw away the last digit.
    counter++;
  }
  uint16_t index = cpu.I;
  for (int i = counter - 1; i >= 0; --i) {
    memory[index] = digits[i];
    index++;
  }
  cpu.pc += 2;
}

void chip8_execute_Fx55(int reg) {
  for (int i = 0; i <= reg; i++) {
    memory[cpu.I + i] = cpu.V[i];
  }
  cpu.I += cpu.V[reg] + 1;
  cpu.pc += 2;
}

void chip8_execute_Fx65(int reg) {
  for (int i = 0; i <= reg; i++) {
    cpu.V[i] = memory[cpu.I + i];
  }
  cpu.I += cpu.V[reg] + 1;
  cpu.pc += 2;
}

static void chip8_decode_opcode(uint16_t opcode, bool debug) {
  if (debug)
    printf("pc: %x %d\n", cpu.pc, cpu.pc);
  switch (opcode & 0xF000) {
  case 0x000: // for instructions that begin with 0x00
    switch (opcode & 0x00FF) {
    case opcode_CLS:
      chip8_execute_00E0();
      if (debug)
        printf("CLS\n");
      break;
    case opcode_RET:
      chip8_execute_00EE();
      if (debug)
        printf("RET\n");
      break;
    }
    break;
  case opcode_1nnn:
    chip8_execute_1nnn(opcode & 0x0FFF);
    if (debug)
      printf("JP %x\n", opcode & 0x0FFF);
    break;
  case opcode_2nnn:
    chip8_execute_2nnn(opcode & 0x0FFF);
    if (debug)
      printf("CALL %x\n", (opcode & 0x0FFF));
    break;
  case opcode_3xkk:
    chip8_execute_3xkk((opcode & 0x0F00) >> 8, opcode & 0x00FF);
    if (debug) {

      printf("SE %d, %d\n", (opcode & 0x0F00) >> 8, opcode & 0x00FF);
      printf("%d, == %d\n", cpu.V[(opcode & 0x0F00) >> 8], opcode & 0x00FF);
    }
    break;
  case opcode_4xkk:
    chip8_execute_4xkk((opcode & 0x0F00) >> 8, opcode & 0x00FF);
    if (debug) {
      printf("SNE %d, %d\n", (opcode & 0x0F00) >> 8, opcode & 0x00FF);
      printf("%d, != %d\n", cpu.V[(opcode & 0x0F00) >> 8], opcode & 0x00FF);
    }
    break;
  case opcode_5xy0:
    chip8_execute_5xy0((opcode & 0x0F00) >> 8, (opcode & 0x00F0) >> 4);
    if (debug) {
      printf("SE %d, %d\n", (opcode & 0x0F00) >> 8, (opcode & 0x00F0) >> 4);
      printf("%d, == %d\n", cpu.V[(opcode & 0x0F00) >> 8],
             cpu.V[(opcode & 0x00F0) >> 4]);
    }
    break;
  case opcode_6xkk:
    chip8_execute_6xkk((opcode & 0x0F00) >> 8, opcode & 0x00FF);
    if (debug) {
      printf("LD %d, %d\n", (opcode & 0x0F00) >> 8, opcode & 0x00FF);
      printf("reg: %d = %d\n", (opcode & 0x0F00) >> 8,
             cpu.V[(opcode & 0x0F00) >> 8]);
    }
    break;
  case opcode_7xkk:
    chip8_execute_7xkk((opcode & 0x0F00) >> 8, opcode & 0x00FF);
    if (debug) {
      printf("ADD %d, %d\n", (opcode & 0x0F00) >> 8, opcode & 0x00FF);
      printf("reg: %d = %d\n", (opcode & 0x0F00) >> 8,
             cpu.V[(opcode & 0x0F00) >> 8]);
    }
    break;
  case opcode_8000:
    switch (opcode & 0x000F) {
    case opcode_8xy0:
      chip8_execute_8xy0((opcode & 0x0F00) >> 8, (opcode & 0x00F0) >> 4);
      if (debug)
        printf("LD %d, %x\n", (opcode & 0x0F00) >> 8, (opcode & 0x00F0) >> 4);
      break;
    case opcode_8xy1:
      chip8_execute_8xy1((opcode & 0x0F00) >> 8, (opcode & 0x00F0) >> 4);
      if (debug)
        printf("OR %x, %x\n", (opcode & 0x0F00) >> 8, (opcode & 0x00F0) >> 4);
      break;
    case opcode_8xy2:
      chip8_execute_8xy2((opcode & 0x0F00) >> 8, (opcode & 0x00F0) >> 4);
      if (debug)
        printf("AND %d, %d\n", (opcode & 0x0F00) >> 8, (opcode & 0x00F0) >> 4);
      break;
    case opcode_8xy3:
      chip8_execute_8xy3((opcode & 0x0F00) >> 8, (opcode & 0x00F0) >> 4);
      if (debug)
        printf("XOR: %x, %x\n", (opcode & 0x0F00) >> 8, (opcode & 0x00F0) >> 4);
      break;
    case opcode_8xy4:
      chip8_execute_8xy4((opcode & 0x0F00) >> 8, (opcode & 0x00F0) >> 4);
      if (debug)
        printf("ADD %d, %d\n", (opcode & 0x0F00) >> 8, (opcode & 0x00F0) >> 4);
      break;
    case opcode_8xy5:
      chip8_execute_8xy5((opcode & 0x0F00) >> 8, (opcode & 0x00F0) >> 4);
      if (debug)
        printf("SUB %x, %x\n", (opcode & 0x0F00) >> 8, (opcode & 0x00F0) >> 4);
      break;
    case opcode_8xy6:
      chip8_execute_8xy6((opcode & 0x0F00) >> 8, (opcode & 0x00F0) >> 4);
      if (debug)
        printf("SHR %x, %x\n", (opcode & 0x0F00) >> 8, (opcode & 0x00F0) >> 4);
      break;
    case opcode_8xy7:
      chip8_execute_8xy7((opcode & 0x0F00) >> 8, (opcode & 0x00F0) >> 4);
      if (debug)
        printf("SUBN %x, %x\n", (opcode & 0x0F00) >> 8, (opcode & 0x00F0) >> 4);
      break;
    case opcode_8xye:
      chip8_execute_8xye((opcode & 0x0F00) >> 8, (opcode & 0x00F0) >> 4);
      if (debug)
        printf("SHL %x, %x\n", (opcode & 0x0F00) >> 8, (opcode & 0x00F0) >> 4);
      break;
    default:
      printf("Unknown opcode\n");
      break;
    }
    break;
  case opcode_9xy0:
    chip8_execute_9xy0((opcode & 0x0F00) >> 8, (opcode & 0x00F0) >> 4);
    if (debug)
      printf("SNE %x, %x\n", (opcode & 0x0F00) >> 8, (opcode & 0x00F0) >> 4);
    break;
  case opcode_Annn:
    chip8_execute_Annn(opcode & 0x0FFF);
    if (debug) {
      printf("LD I, %x\n", opcode & 0x0FFF);
      printf("I: %x\n", cpu.I);
    }
    break;
  case opcode_Bnnn:
    chip8_execute_Bnnn(opcode & 0x0FFF);
    if (debug)
      printf("JP %x, %x", 0, opcode & 0x0FFF);
    break;
  case opcode_Cxkk:
    chip8_execute_Cxkk((opcode & 0x0F00) >> 8, opcode & 0x00FF);
    if (debug)
      printf("RND %d, %d\n", (opcode & 0x0F00) >> 8, opcode % 0x00FF);
    break;
  case opcode_Dxyn:
    /* SDL_Delay(10); */
    chip8_execute_Dxyn((opcode & 0x0F00) >> 8, (opcode & 0x00F0) >> 4,
                       opcode & 0x000F);
    if (debug) {

      printf("reg1: %d reg2: %d\n", (opcode & 0x0F00) >> 8,
             (opcode & 0x00F0) >> 4);
      printf("x: %d y: %d height: %d\n", cpu.V[(opcode & 0x0F00) >> 8],
             cpu.V[(opcode & 0x00F0) >> 4], opcode & 0x000F);
    }
    break;
  case opcode_E000:
    switch (opcode & 0x00FF) {
    case opcode_Ex9e:
      chip8_execute_Ex9e((opcode & 0x0F00) >> 8);
      break;
    case opcode_Exa1:
      chip8_execute_Exa1((opcode & 0x0F00) >> 8);
      break;
    default:
      printf("Unknown opcode\n");
      break;
    }
    break;
  case opcode_F000:
    switch (opcode & 0x00FF) {
    case opcode_Fx07:
      chip8_execute_Fx07((opcode & 0x0F00) >> 8);
      if (debug)
        printf("register = %d delay timer: %d\n", (opcode & 0x0F00) >> 8,
               cpu.dt);
      break;
    case opcode_Fx0a:
      chip8_execute_Fx0a((opcode & 0x0F00) >> 8);
      break;
    case opcode_Fx15:
      chip8_execute_Fx15((opcode & 0x0F00) >> 8);
      if (debug)
        printf("delay timer = %d, register: %d\n", cpu.dt,
               (opcode & 0x0F00) >> 8);
      break;
    case opcode_Fx18:
      chip8_execute_Fx18((opcode & 0x0F00) >> 8);
      if (debug)
        printf("sound timer = %d, register: %d\n", cpu.st,
               (opcode & 0x0F00) >> 8);
      break;
    case opcode_Fx1e:
      chip8_execute_Fx1e((opcode & 0x0F00) >> 8);
      if (debug)
        printf("index register = %d, register: %d\n", cpu.I,
               (opcode & 0x0F00) >> 8);
      break;
    case opcode_Fx29:
      chip8_execute_Fx29((opcode & 0x0F00) >> 8);
      break;
    case opcode_Fx33:
      chip8_execute_Fx33((opcode & 0x0F00) >> 8);
      if (debug)
        printf("LD B, %d", (opcode & 0x0F00) >> 8);
      break;
    case opcode_Fx55:
      chip8_execute_Fx55((opcode & 0x0F00) >> 8);
      break;
    case opcode_Fx65:
      chip8_execute_Fx65((opcode & 0x0F00) >> 8);
      break;
    default:
      printf("Unknown opcode\n");
      break;
    }
    break;
  default:
    printf("Unknown opcode\n");
    break;
  }
}
static void chip8_decode_opcode_debug(uint16_t opcode) {
  chip8_decode_opcode(opcode, true);
}

void chip8_print_insn() {
  uint16_t insn = chip8_fetch_opcode(cpu.pc);

  printf("opcode: %x\n", insn);
  chip8_decode_opcode_debug(insn);
}

void chip8_start_instruction_cycle() {
  uint16_t insn = chip8_fetch_opcode(cpu.pc);
  chip8_decode_opcode(insn, false);
}

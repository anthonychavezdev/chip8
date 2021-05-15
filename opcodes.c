#include "opcodes.h"
#include <unistd.h>

void chip8_print_insn(int rom_size) {
  uint16_t insn = chip8_fetch_opcode(cpu.pc);

  chip8_decode_opcode(insn);
}

uint16_t chip8_fetch_opcode(uint16_t addr) {
  return memory[addr] << 8 | memory[addr + 1];
}
void chip8_decode_opcode(uint16_t opcode) {
  printf("opcode: %04x\npc: %04x\n", opcode, cpu.pc);

  switch (opcode & 0xF000) {
  case 0x000: // for instructions that begin with 0x00
  case opcode_CLS:
    chip8_execute_00E0();
    break;
  case opcode_RET:
    chip8_execute_00EE();
    break;
    break;
  case opcode_1nnn:
    chip8_execute_1nnn(opcode & 0x0FFF);
    break;
  case opcode_2nnn:
    chip8_execute_2nnn(opcode & 0x0FFF);
  case opcode_3xkk:
    chip8_execute_3xkk((opcode & 0x0F00) >> 8, opcode & 0x00FF);
    break;
  case opcode_4xkk:
    chip8_execute_4xkk((opcode & 0x0F00) >> 8, opcode & 0x00FF);
    break;
  case opcode_5xy0:
    chip8_execute_5xy0((opcode & 0x0F00) >> 8, (opcode & 0x00F0) >> 4);
    break;
  case opcode_6xkk:
    chip8_execute_6xkk((opcode & 0x0F00) >> 8, opcode & 0x00FF);
    break;
  case opcode_7xkk:
    chip8_execute_7xkk((opcode & 0x0F00) >> 8, opcode & 0x00FF);
    break;
  case opcode_8000:
    switch (opcode & 0x000F) {
    case opcode_8xy0:
      chip8_execute_8xy0((opcode & 0x0F00) >> 8, (opcode & 0x00F0) >> 4);
      break;
    case opcode_8xy1:
      chip8_execute_8xy1((opcode & 0x0F00) >> 8, (opcode & 0x00F0) >> 4);
      break;
    case opcode_8xy2:
      chip8_execute_8xy2((opcode & 0x0F00) >> 8, (opcode & 0x00F0) >> 4);
      break;
    case opcode_8xy3:
      chip8_execute_8xy3((opcode & 0x0F00) >> 8, (opcode & 0x00F0) >> 4);
      break;
    case opcode_8xy4:
      chip8_execute_8xy4((opcode & 0x0F00) >> 8, (opcode & 0x00F0) >> 4);
      break;
    case opcode_8xy5:
      chip8_execute_8xy5((opcode & 0x0F00) >> 8, (opcode & 0x00F0) >> 4);
      break;
    case opcode_8xy6:
      chip8_execute_8xy6((opcode & 0x0F00) >> 8, (opcode & 0x00F0) >> 4);
      break;
    case opcode_8xy7:
      chip8_execute_8xy7((opcode & 0x0F00) >> 8, (opcode & 0x00F0) >> 4);
      break;
    case opcode_8xye:
      chip8_execute_8xye((opcode & 0x0F00) >> 8, (opcode & 0x00F0) >> 4);
      break;
    default:
      printf("Unknown opcode\n");
      break;
    }
    break;
  case opcode_9xy0:
    chip8_execute_9xy0((opcode & 0x0F00) >> 8, (opcode & 0x00F0) >> 4);
    break;
  case opcode_Annn:
    chip8_execute_Annn(opcode & 0x0FFF);
    break;
  case opcode_Bnnn:
    chip8_execute_Bnnn(opcode & 0x0FFF);
    break;
  case opcode_Cxkk:
    chip8_execute_Cxkk((opcode & 0x0F00) >> 8, opcode & 0x00FF);
    break;
  case opcode_Dxyn:
    chip8_execute_Dxyn((opcode & 0x0F00) >> 8, (opcode & 0x00F0) >> 4,
                       opcode & 0x000F);
    break;
  case opcode_E000:
    switch (opcode & 0x000F) {
    case opcode_Ex9e:
      chip8_execute_Ex9e((opcode & 0x0F00) >> 8);
      break;
    case opcode_Exa1:
      chip8_execute_Exa1((opcode & 0x0F00) >> 8);
      break;
    }
    break;
  case opcode_F000:
    switch (opcode & 0x000F) {
    case opcode_Fx07:
      chip8_execute_Fx07((opcode & 0x0F00) >> 8);
      break;
    case opcode_Fx0a:
      chip8_execute_Fx0a((opcode & 0x0F00) >> 8);
      break;
    case opcode_Fx15:
      chip8_execute_Fx15((opcode & 0x0F00) >> 8);
      break;
    case opcode_Fx18:
      chip8_execute_Fx18((opcode & 0x0F00) >> 8);
      break;
    case opcode_Fx1e:
      chip8_execute_Fx1e((opcode & 0x0F00) >> 8);
      break;
    case opcode_Fx29:
      chip8_execute_Fx29((opcode & 0x0F00) >> 8);
      break;
    case opcode_Fx33:
      chip8_execute_Fx33((opcode & 0x0F00) >> 8);
      break;
    case opcode_Fx55:
      chip8_execute_Fx55((opcode & 0x0F00) >> 8);
      break;
    case opcode_Fx65:
      chip8_execute_Fx65((opcode & 0x0F00) >> 8);
      break;
    }
    break;
  default:
    printf("Unknown opcode\n");
    break;
  }
}

void chip8_execute_00E0() {
  chip8_clear_screen();
  cpu.pc += 2;
}

void chip8_execute_00EE() {
  printf("RET\n");
  cpu.pc = stack[--cpu.sp];
}

void chip8_execute_1nnn(uint16_t addr) {
  printf("JP %x\n", addr);
  cpu.pc = addr;
}

void chip8_execute_2nnn(uint16_t addr) {
  stack[cpu.sp++] = cpu.pc + 2;
  cpu.pc = addr;
  printf("CALL %x\n", addr);
}
void chip8_execute_3xkk(int reg, int kk) {
  cpu.pc += (cpu.V[reg] == kk) ? 4 : 2;
  printf("SE %d, %x\n", reg, kk);
}

void chip8_execute_4xkk(int reg, int kk) {
  cpu.pc += (cpu.V[reg] != kk) ? 4 : 2;
  printf("SNE reg1: %d, value: %d\n", reg, kk);
}

void chip8_execute_5xy0(int reg1, int reg2) {
  cpu.pc += (cpu.V[reg1] == cpu.V[reg2]) ? 4 : 2;
  printf("SE %d, %d\n", reg1, reg2);
}

void chip8_execute_6xkk(int reg, int kk) {
  cpu.V[reg] = kk;
  cpu.pc += 2;
  printf("LD %d, %d\n", reg, kk);
  printf("reg: %d = %d\n", reg, cpu.V[reg]);
}

void chip8_execute_7xkk(int reg, int kk) {
  cpu.V[reg] += kk;
  cpu.pc += 2;
  printf("ADD %d, %d\n", reg, kk);
  printf("reg: %d = %d\n", reg, cpu.V[reg]);
}

void chip8_execute_8xy0(int reg1, int reg2) {
  printf("LD %d, %x\n", reg1, reg2);
  cpu.V[reg1] = cpu.V[reg2];
  cpu.pc += 2;
}

void chip8_execute_8xy1(int reg1, int reg2) {
  printf("OR %x, %x\n", reg1, reg2);
  cpu.V[reg1] |= cpu.V[reg2];
  cpu.pc += 2;
}

void chip8_execute_8xy2(int reg1, int reg2) {
  printf("AND %x, %x\n", reg1, reg2);
  cpu.V[reg1] &= cpu.V[reg2];
  cpu.pc += 2;
}

void chip8_execute_8xy3(int reg1, int reg2) {
  printf("XOR: %x, %x\n", reg1, reg2);
  cpu.V[reg1] ^= cpu.V[reg2];
  cpu.pc += 2;
}

void chip8_execute_8xy4(int reg1, int reg2) {
  printf("ADD %x, %x\n", reg1, reg2);
  cpu.V[0xF] = (cpu.V[reg1] + cpu.V[reg2]) > 255 ? 1 : 0;
  cpu.V[reg1] += cpu.V[reg2];
  cpu.pc += 2;
}

void chip8_execute_8xy5(int reg1, int reg2) {
  printf("SUB %x, %x\n", reg1, reg2);
  cpu.V[0xf] = (cpu.V[reg1] > cpu.V[reg2]) ? 1 : 0;
  cpu.V[reg1] -= cpu.V[reg2];
  cpu.pc += 2;
}

void chip8_execute_8xy6(int reg1, int reg2) {
  printf("SHR %x, %x\n", reg1, reg2);
  (void)reg2;
  cpu.V[0xf] = (cpu.V[reg1] & 0x01);
  cpu.V[reg1] >>= 1;
  cpu.pc += 2;
}
void chip8_execute_8xy7(int reg1, int reg2) {
  printf("SUBN %x, %x\n", reg1, reg2);
  cpu.V[0xf] = (cpu.V[reg2] > cpu.V[reg1]) ? 1 : 0;
  cpu.V[reg2] -= cpu.V[reg1];
  cpu.pc += 2;
}

void chip8_execute_8xye(int reg1, int reg2) {
  printf("SHL %x, %x\n", reg1, reg2);
  (void)reg2;
  cpu.V[0xf] = (cpu.V[reg1] & 0x80) >> 7;
  cpu.V[reg1] <<= 1;
  cpu.pc += 2;
}

void chip8_execute_9xy0(int reg1, int reg2) {
  printf("SNE %x, %x\n", reg1, reg2);
  cpu.pc += (cpu.V[reg1] != cpu.V[reg2]) ? 4 : 2;
}

void chip8_execute_Annn(uint16_t addr) {
  cpu.I = addr;
  cpu.pc += 2;
  printf("LD I, %x\n", addr);
  printf("I: %x\n", cpu.I);
}

void chip8_execute_Bnnn(uint16_t addr) {
  printf("JP %x, %x", 0, addr);
  cpu.pc = (addr + cpu.V[0]);
}

void chip8_execute_Cxkk(int reg, int kk) {
  srand(time(NULL));
  uint8_t rand_num = rand();
  cpu.V[reg] = (rand_num & kk);
  cpu.pc += 2;
  printf("register: %d random number: %d\n", reg, rand_num);
}

void chip8_execute_Dxyn(int reg1, int reg2, int n) {
  uint8_t pixel;
  uint8_t x = cpu.V[reg1] % 64;
  uint8_t y = cpu.V[reg2] % 32;
  uint8_t height = n;

  cpu.V[0xF] = 0;

  for (int yline = 0; yline < height; yline++) {
    pixel = memory[cpu.I + yline];

    for (int xline = 0; xline < 8; xline++) {
      if ((pixel & (0x80 >> xline)) != 0) {
        if (display[(x + xline + ((y + yline) * 64))] > 0) {
          cpu.V[0xF] = 1;
        }
        display[x + xline + ((y + yline) * 64)] ^= 1;
      } else {
        display[x + xline + ((y + yline) * 64)] = 0;
      }
    }
  }
  printf("reg1: %d reg2: %d\n", reg1, reg2);
  printf("x: %d y: %d height: %d\n", x, y, height);
  chip8_draw_screen();
  cpu.pc += 2;
}

void chip8_execute_Ex9e(int reg) { cpu.pc += 4; }

void chip8_execute_Exa1(int reg) { cpu.pc += 4; }

void chip8_execute_Fx07(int reg) {
  cpu.V[reg] = cpu.dt;
  printf("register = %d delay timer: %d\n", reg, cpu.dt);
  cpu.pc += 2;
}

void chip8_execute_Fx0a(int reg) {
  SDL_Delay(1);
  cpu.pc += 2;
}

void chip8_execute_Fx15(int reg) {
  cpu.dt = cpu.V[reg];
  cpu.pc += 2;
  printf("delay timer = %d, register: %d\n", cpu.dt, reg);
}

void chip8_execute_Fx18(int reg) {
  cpu.st = cpu.V[reg];
  cpu.pc += 2;
  printf("sound timer = %d, register: %d\n", cpu.st, reg);
}

void chip8_execute_Fx1e(int reg) {
  cpu.V[0xf] = (cpu.I + cpu.V[reg] > 0xfff) ? 1 : 0;
  cpu.I += cpu.V[reg];
  printf("index register = %d, register: %d", cpu.I, reg);
  cpu.pc += 2;
}

void chip8_execute_Fx29(int reg) {
  cpu.I = FONTSET_BYTES_PER_CHAR * cpu.V[reg];
  cpu.pc += 2;
}

void chip8_execute_Fx33(int reg) {
  uint8_t num = cpu.V[reg];
  uint8_t digits[3];
  uint8_t counter = 0;
  while (num > 0) {
    uint8_t mod = num % 10; // grab last digit
    digits[counter] = mod;
    num /= 10; // Throw away the last digit.
    counter++;
  }
  for (uint8_t i = counter - 1; i >= 0; i--) {
    memory[cpu.I] = digits[i];
    cpu.I++;
    printf("index reg: %d, digit: %d", cpu.I, digits[i]);
  }
  cpu.pc += 2;
}

void chip8_execute_Fx55(int reg) {
  for (int i = 0; i <= reg; i++) {
    memory[cpu.I + i] = cpu.V[i];
    printf("index reg value: %d, reg: %d, reg value: %d\n", memory[cpu.I], i,
           cpu.V[i]);
  }
  cpu.I += cpu.V[reg] + 1;
  cpu.pc += 2;
}

void chip8_execute_Fx65(int reg) {
  for (int i = 0; i <= reg; i++) {
    cpu.V[i] = memory[cpu.I + i];
    printf("reg: %d reg value: %d, index reg value: %d\n", i, cpu.V[i],
           memory[cpu.I]);
  }
  cpu.I += cpu.V[reg] + 1;
  cpu.pc += 2;
}

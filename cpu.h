#ifndef CPU_H
#define CPU_H

#include <stdint.h>

#define NUM_OF_GPRS  16 // How many general purpose registers there are.

#define opcode_CLS 0x00E0  // CLS: Clears screen
#define opcode_RET 0x00EE  // RET: Return from a subroutine
#define opcode_1nnn 0x1000 // JP addr: Jump to location nnn
#define opcode_2nnn 0x2000 // CALL addr: Call subroutine at nnn
#define opcode_3xkk 0x3000 // SE Vx, Byte: Skip next instruction if Vx == kk
#define opcode_4xkk 0x4000 // SNE Vx, byte: Skip next instruction if Vx != kk
#define opcode_5xy0 0x5000 // SE Vx, Vy: Skip next instruction Vx == Vy
#define opcode_6xkk 0x6000 // LD Vx, byte: Set Vx = kk
#define opcode_7xkk 0x7000 // ADD Vx, byte: Set Vx = Vx + kk
#define opcode_8000 0x8000 // for opcodes that begin with 0x8000
//**************************************************************
#define opcode_8xy0 0x0000 // LD Vx, Vy: Set Vx = Vy
#define opcode_8xy1 0x0001 // OR Vx, Vy: Set Vx = Vx OR Vy
#define opcode_8xy2 0x0002 // AND Vx, Vy: Set Vx = Vx AND Vy
#define opcode_8xy3 0x0003 // XOR Vx, Vy: Set Vx = Vx XOR Vy
#define opcode_8xy4 0x0004 // ADD Vx, Vy: Set Vx = Vx + Vy, set VF == carry
#define opcode_8xy5 0x0005 // SUB Vx, Vy: Set Vx = Vx - Vy, set VF == !borrow
#define opcode_8xy6 0x0006 // SHR Vx {, Vy}: Set Vx = Vx SHR 1, set VF == Vx & 1
#define opcode_8xy7 0x0007 // SUBN Vx Vy: Set Vx = Vy - Vx, set VF == !borrow
#define opcode_8xye 0x000E // SHL Vx {, Vy}: Set Vx = Vx SHL 1, set VF == Vx & 8000
//**************************************************************
#define opcode_9xy0 0x9000 // SNE Vx, Vy: Skip next instruction if Vx != Vy
#define opcode_Annn 0xA000 // LD I, addr: Set I = nnn
#define opcode_Bnnn 0xB000 // JP V0, addr: Jump to location nnn + V0
#define opcode_Cxkk 0xC000 // RND Vx, byte: Set Vx = random byte & kk
#define opcode_Dxyn 0xD000 // DRW Vx, Vy, nibble: Display n-byte sprite starting at memory location I at (Vx, Vy), set VF = collision.
#define opcode_E000 0xE000 // for opcodes that begin with 0xE000
//**************************************************************
#define opcode_Ex9e 0x009e // Skip Vx: Skip next instruction if key with the value of Vx is pressed.
#define opcode_Exa1 0x00a1 // SKNP Vx: Skip next instruction if key with the value of Vx is not pressed.
#define opcode_F000 0xF000 // for opcodes that begin with 0xE000
//**************************************************************
#define opcode_Fx07 0x0007 // LD Vx, DT: Set Vx = delay timer value.
#define opcode_Fx0a 0x000A // LD Vx, K: Wait for a key press, store the value of the key in Vx.
#define opcode_Fx15 0x0015 // LD DT, Vx: Set delay timer = Vx.
#define opcode_Fx18 0x0018 // LD ST, Vx: Set sound timer = Vx.
#define opcode_Fx1e 0x001E // ADD I, Vx: Set I = I + Vx.
#define opcode_Fx29 0x0029 // LD F, Vx: Set I = location of sprite for digit Vx.
#define opcode_Fx33 0x0033 // LD B, Vx: Store BCD representation of Vx in memory locations I, I+1, and I+2.
#define opcode_Fx55 0x0055 // LD [I], Vx: Store registers V0 through Vx in memory starting at location I.
#define opcode_Fx65 0x0065 // LD Vx, [I]: Read registers V0 through Vx from memory starting at location I.
//**************************************************************

struct cpu
{
    uint8_t V[NUM_OF_GPRS]; // 16 general purpose registers. 0x0-0xF
    uint8_t dt;             // delay timer register
    uint8_t st;             // sound timer register
    uint16_t I;             // holds memory addresses. (rightmost 12 bits)
    uint16_t pc;            // Program counter. Not accessible to programs
    uint16_t sp;            // holds 16 values. Points to the topmost item
};
extern struct cpu cpu;

void chip8_init_regs();
void chip8_update_timers();

/**
 * CLS: Clears screen
 */
void chip8_execute_00E0();
/**
 * RET: Return from a subroutine
 */
void chip8_execute_00EE();
/**
 * JP addr: Jump to location nnn
 */
void chip8_execute_1nnn(uint16_t addr);
/**
 * CALL addr: Call subroutine at nnn
 */
void chip8_execute_2nnn(uint16_t addr);
/**
 * SE Vx, Byte: Skip next instruction if Vx == kk
 */
void chip8_execute_3xkk(int reg, int kk);
/**
 * SNE Vx, byte: Skip next instruction if Vx != kk
 */
void chip8_execute_4xkk(int reg, int kk);
/**
 * SE Vx, Vy: Skip next instruction Vx == Vy
 */
void chip8_execute_5xy0(int reg1, int reg2);
/**
 * LD Vx, byte: Set Vx = kk
 */
void chip8_execute_6xkk(int reg, int kk);
/**
 * ADD Vx, byte: Set Vx = Vx + kk
 */
void chip8_execute_7xkk(int reg, int kk);
/**
 * LD Vx, Vy: Set Vx = Vy
 */
void chip8_execute_8xy0(int reg1, int reg2);
/**
 * OR Vx, Vy: Set Vx = Vx OR Vy
 */
void chip8_execute_8xy1(int reg1, int reg2);
/**
 * AND Vx, Vy: Set Vx = Vx AND Vy
 */
void chip8_execute_8xy2(int reg1, int reg2);
/**
 * XOR Vx, Vy: Set Vx = Vx XOR Vy
 */
void chip8_execute_8xy3(int reg1, int reg2);
/**
 * ADD Vx, Vy: Set Vx = Vx + Vy, set VF == carry
 */
void chip8_execute_8xy4(int reg1, int reg2);
/**
 * SUB Vx, Vy: Set Vx = Vx - Vy, set VF == !borrow
 */
void chip8_execute_8xy5(int reg1, int reg2);
/**
 * SHR Vx {, Vy}: Set Vx = Vx SHR 1, set VF == Vx & 1
 */
void chip8_execute_8xy6(int reg1, int reg2);
/**
 * SUBN Vx Vy: Set Vx = Vy - Vx, set VF == !borrow
 */
void chip8_execute_8xy7(int reg1, int reg2);
/**
 * SHL Vx {, Vy}: Set Vx = Vx SHL 1, set VF == Vx & 8000
 */
void chip8_execute_8xye(int reg1, int reg2);
/**
 * SNE Vx, Vy: Skip next instruction if Vx != Vy
 */
void chip8_execute_9xy0(int reg1, int reg2);
/**
 * LD I, addr: Set I = nnn
 */
void chip8_execute_Annn(uint16_t addr);
/**
 * JP V0, addr: Jump to location nnn + V0
 */
void chip8_execute_Bnnn(uint16_t addr);
/**
 * RND Vx, byte: Set Vx = random byte & kk
 */
void chip8_execute_Cxkk(int reg, int kk);
/**
 * DRW Vx, Vy, nibble: Display n-byte sprite starting at memory location I at (Vx, Vy), set VF = collision.
 */
void chip8_execute_Dxyn(int reg1, int reg2, int n);
/**
 * Skip Vx: Skip next instruction if key with the value of Vx is pressed.
 */
void chip8_execute_Ex9e(int reg);
/**
 * SKNP Vx: Skip next instruction if key with the value of Vx is not pressed.
 */
void chip8_execute_Exa1(int reg);
/**
 * LD Vx, DT: Set Vx = delay timer value.
 */
void chip8_execute_Fx07(int reg);
/**
 * LD Vx, K: Wait for a key press, store the value of the key in Vx.
 */
void chip8_execute_Fx0a(int reg);
/**
 * LD DT, Vx: Set delay timer = Vx.
 */
void chip8_execute_Fx15(int reg);
/**
 * LD ST, Vx: Set sound timer = Vx.
 */
void chip8_execute_Fx18(int reg);
/**
 * ADD I, Vx: Set I = I + Vx.
 */
void chip8_execute_Fx1e(int reg);
/**
 * LD F, Vx: Set I = location of sprite for digit Vx.
 */
void chip8_execute_Fx29(int reg);
/**
 * LD B, Vx: Store BCD representation of Vx in memory locations I, I+1, and I+2.
 */
void chip8_execute_Fx33(int reg);
/**
 * LD [I], Vx: Store registers V0 through Vx in memory starting at location I.
 */
void chip8_execute_Fx55(int reg);
/**
 * LD Vx, [I]: Read registers V0 through Vx from memory starting at location I.
 */
void chip8_execute_Fx65(int reg);

uint16_t chip8_fetch_opcode(uint16_t addr);
/* static void chip8_decode_opcode(uint16_t opcode, bool debug); */
/* static void chip8_decode_opcode_debug(uint16_t opcode); */
void chip8_start_instruction_cycle();

void chip8_print_insn();
#endif

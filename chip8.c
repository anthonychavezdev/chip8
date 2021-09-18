#define _XOPEN_SOURCE 600 // Feature flag to allow nanosleep to be called
#include "chip8.h"
#include "cpu.h"
#include "graphics.h"
#include "memory.h"
#include <getopt.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

void usage() {
  fprintf(stderr, "Usage: chip8 [-d] -f rom\n");
  fprintf(stderr, "    -d put the chip8 emulator in debug mode\n");
  fprintf(stderr, "    -f Specify which ROM to load\n");
  fprintf(stderr,
          "    -s Set the emulator speed in miliseconds. (Default is 1)\n");
  exit(1);
}
int chip8_load_rom(char *path) {
  FILE *rom;
  rom = fopen(path, "rb");
  if (rom == NULL) {
    fprintf(stderr, "Unable to open rom: %s\n", path);
    exit(1);
  }
  int rom_size = fread(&memory[0x200], 1, MAX_ROM_SIZE, rom);
  fclose(rom);

  return rom_size;
}

// wrapper for nanosleep
int msleep(long miliseconds) {
  struct timespec req, rem;

  if (miliseconds > 999) {
    req.tv_sec = (int)(miliseconds / 1000); /* Must be Non-Negative */
    req.tv_nsec = (miliseconds - ((long)req.tv_sec * 1000)) *
                  1000000; /* Must be in range of 0 to 999999999 */
  } else {
    req.tv_sec = 0; /* Must be Non-Negative */
    req.tv_nsec =
        miliseconds * 1000000; /* Must be in range of 0 to 999999999 */
  }

  return nanosleep(&req, &rem);
}

int main(int argc, char *argv[]) {
  bool debug_mode_enabled = false;
  int opt;
  int rom_size;
  int miliseconds = 1;
  bool running = true;
  char *file;
  chip8_init_memory();
  chip8_init_stack();
  chip8_init_regs();
  chip8_init_graphics();

  while ((opt = getopt(argc, argv, "df:s:")) != -1) {
    switch (opt) {
    case 'd':
      /* print_insn(); */
      debug_mode_enabled = true;
      break;
    case 'f':
      file = optarg;
      break;
    case 's':
      miliseconds = atoi(optarg);
      break;

    default: /* '?' */
      usage();
    }
  }

  if (optind < argc)
    usage(); // missing rom

  rom_size = chip8_load_rom(file);
  (void)rom_size;

  while (running) {
    if (debug_mode_enabled) {
      msleep(miliseconds);
      chip8_dump_memory(memory);
      chip8_print_insn();
    }
    msleep(miliseconds);
    chip8_start_instruction_cycle();
    chip8_manage_events(&running);
    chip8_update_timers();
  }

  chip8_destory_graphics();
  return 0;
}

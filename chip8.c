#include "chip8.h"

static void usage() {
  fprintf(stderr, "Usage: chip8 [-d] rom\n");
  fprintf(stderr, "    -d put the chip8 emulator in debug mode\n");
  exit(1);
}
int chip8_load_rom(char *path) {
  FILE *rom;
  rom = fopen(path, "rb");
  if (rom == NULL) {
    fprintf(stderr, "Unable to open rom: %s", path);
    exit(1);
  }
  int rom_size = fread(&memory[0x200], 1, MAX_ROM_SIZE, rom);
  fclose(rom);

  return rom_size;
}

int main(int argc, char *argv[]) {
  bool debug_mode_enabled = false;
  chip8_init_memory();
  chip8_init_stack();
  chip8_init_regs();
  chip8_init_graphics();

  int opt;
  while ((opt = getopt(argc, argv, "d")) != -1) {
    switch (opt) {
    case 'd':
      /* print_insn(); */
      debug_mode_enabled = true;
      break;

    default: /* '?' */
      usage();
    }
  }

  if (optind >= argc)
    usage(); // missing rom

  int rom_size = chip8_load_rom(argv[optind]);
  bool running = true;

  while (running) {
    if (debug_mode_enabled) {
      chip8_dump_memory(memory);
      chip8_print_insn();
    }
    /* chip8_start_instruction_cycle(); */
    chip8_print_insn();
    chip8_update_timers();
    chip8_manage_events(&running);
  }

  chip8_destory_graphics();
  return 0;
}

#include <stdint.h>
#include <SDL2/SDL.h>
#include <stdbool.h>
// Keymap for the emulator. Comments are the orignal
// key on the hex keypad
#define NUM_KEYS 16

extern uint8_t KEYMAP[NUM_KEYS];
extern uint8_t keys[NUM_KEYS];
extern bool key_pressed;
/*
* processes user input
* @param A pointer of SDL_Event that us used to read events.
*/
void chip8_process_input(SDL_Event *e);

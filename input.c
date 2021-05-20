#include "input.h"

uint8_t KEYMAP[NUM_KEYS] = {
    SDLK_x, // 0
    SDLK_1, // 1
    SDLK_2, // 2
    SDLK_3, // 3
    SDLK_q, // 4
    SDLK_w, // 5
    SDLK_e, // 6
    SDLK_a, // 7
    SDLK_s, // 8
    SDLK_d, // 9
    SDLK_z, // A
    SDLK_c, // B
    SDLK_4, // C
    SDLK_r, // D
    SDLK_f, // E
    SDLK_v  // F
};
uint8_t keys[NUM_KEYS] = {0};

void chip8_process_input(SDL_Event *e) {
  switch (e->type) {
  case SDL_KEYDOWN:
    for (int i = 0; i < NUM_KEYS; i++)
      if (e->key.keysym.sym == KEYMAP[i])
        keys[i] = true;

    /* for (int i = 0; i < NUM_KEYS; i++) */
    /*   printf("Key %c: %d\n", KEYMAP[i], keys[i]); */
    break;
  case SDL_KEYUP:
    for (int i = 0; i < NUM_KEYS; i++)
      if (e->key.keysym.sym == KEYMAP[i])
        keys[i] = false;
    break;
  }
}

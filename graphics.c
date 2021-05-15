#include "graphics.h"

uint32_t display[DISPLAY_SIZE];
uint8_t FONT_SET[] = {
    0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
    0x20, 0x60, 0x20, 0x20, 0x70, // 1
    0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
    0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
    0x90, 0x90, 0xF0, 0x10, 0x10, // 4
    0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
    0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
    0xF0, 0x10, 0x20, 0x40, 0x40, // 7
    0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
    0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
    0xF0, 0x90, 0xF0, 0x90, 0x90, // A
    0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
    0xF0, 0x80, 0x80, 0x80, 0xF0, // C
    0xE0, 0x90, 0x90, 0x90, 0xE0, // D
    0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
    0xF0, 0x80, 0xF0, 0x80, 0x80  // F
};
SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;
SDL_Texture *texture = NULL;
SDL_Event event;
SDL_Rect sprite;

uint32_t frame_buffer[DISPLAY_SIZE] = {0};

void chip8_init_graphics() {
  for (int i = 0; i < 80; i++)
    memory[i] = FONT_SET[i];

  memset(display, 0, sizeof(uint8_t) * DISPLAY_SIZE);

  if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO))
    fprintf(stderr, "Error initiating the graphics.\n%s", SDL_GetError());

  window = SDL_CreateWindow(
      "chip8", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 480,
      SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);
  if (window == NULL) {
    fprintf(stderr, "Error initiating the graphics.\n%s", SDL_GetError());
    exit(1);
  }
  renderer = SDL_CreateRenderer(window, -1, 0);
  if (renderer == NULL) {
    fprintf(stderr, "Error initiating the graphics.\n%s", SDL_GetError());
    exit(1);
  }
  texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888,
                              SDL_TEXTUREACCESS_STREAMING, DISPLAY_WIDTH,
                              DISPLAY_HEIGHT);
  if (texture == NULL) {
    fprintf(stderr, "Error initiating the graphics.\n%s", SDL_GetError());
    exit(1);
  }
}
void chip8_manage_events(bool *running) {
  while (SDL_PollEvent(&event) != 0) {
    switch (event.type) {
    case SDL_QUIT:
      *running = false;
      break;
    default:
      printf("");
    }
  }
}
void chip8_clear_screen() {
  memset(display, 0, sizeof(display));
  memset(frame_buffer, 0, sizeof(display));
}
void chip8_draw_screen() {
  for (int i = 0; i < DISPLAY_SIZE; i++) {
    uint8_t pixel = display[i];
    frame_buffer[i] = (0x00FFFFFF * pixel) | 0xFF000000;
  }

  SDL_UpdateTexture(texture, NULL, frame_buffer, DISPLAY_WIDTH * 4);
  SDL_RenderClear(renderer);
  SDL_RenderCopy(renderer, texture, NULL, NULL);
  SDL_RenderPresent(renderer);
}

void chip8_destory_graphics() {
  SDL_DestroyTexture(texture);
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
}

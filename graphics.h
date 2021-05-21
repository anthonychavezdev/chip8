#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <SDL2/SDL.h>
#include <stdbool.h>


#define DISPLAY_WIDTH 64
#define DISPLAY_HEIGHT 32
#define DISPLAY_SIZE 64 * 32
#define PIXEL_ON 0xFFFFFFFF
#define PIXEL_OFF 0xFF000000
#define FONTSET_BYTES_PER_CHAR 5

extern uint32_t display[DISPLAY_SIZE];

extern uint8_t FONT_SET[];

extern SDL_Window *window;
extern SDL_Event event;
extern SDL_Renderer *renderer;
extern SDL_Texture *texture;
extern SDL_Rect sprite;
extern uint32_t frame_buffer[2048];

void chip8_init_graphics();
void chip8_manage_events(bool *running);
void chip8_clear_screen();
void chip8_draw_screen();
void chip8_destory_graphics();

#endif

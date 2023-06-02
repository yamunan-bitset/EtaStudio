#ifndef __ETA_STUDIO__H__
#define __ETA_STUDIO__H__

#include <stdbool.h>
#include <stdint.h>
#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include <SDL2/SDL_ttf.h>

typedef struct vec {
    int x, y;
} Vec;

typedef struct color {
    uint8_t r;
    uint8_t g;
    uint8_t b;
    uint8_t a;
} Color;

typedef struct screen {
    SDL_Renderer* impl;
    SDL_Window* window;
    Vec dim;
    Vec offset;
    struct color bg;
    bool fullscreen;
    bool done;
    SDL_Event event;
} Screen;

typedef struct keyboard {
    const uint8_t* keys;
    int states[256];
} Key;

typedef struct msgs {
    const char* str;
    Vec pos;
    TTF_Font* font;
    Color c;
} Msg;

typedef struct tex {
    SDL_Texture* tex;
    SDL_Surface* sur;
    Vec pos;
} Tex;

typedef struct box {
    Msg* msgs;
    Tex* texs;
    Vec top, bottom;
} Box;

#define xy(x, y) Vec({x, y})
#define col(r, g, b, a) Color({r, g, b, a})
#define RGB(c) SDL_Color({c.r, c.g, c.b})
#define RGBA(c) SDL_Color({c.r, c.g, c.b, 255})


void init_sdl();
void close_sdl(Screen*);
void setup_screen(Screen*);
void render_sdl(Screen*);
void update_sdl(Screen*);

void handle_type(Screen*, int, void(*)());
void handle_key(Screen*, int, void(*)());

void draw_quad(Screen*, Vec, Vec, Color);
void draw_circ(Screen*, Vec, float, Color);
void draw_box_solid(Screen*, Vec, Vec, Color);
void draw_box(Screen*, Box*, Color);
SDL_Texture* gen_text(Screen*, const char*, TTF_Font*, Color);
void draw_texture(Screen*, SDL_Texture*, Vec);
void draw_text(Screen*, Msg*);

void message_box(const Screen*, const char*, const char*);
void exit_with_error_msg(const Screen*, const char*);
const char* get_error_msgs();
void error_msg(const char*);

#endif
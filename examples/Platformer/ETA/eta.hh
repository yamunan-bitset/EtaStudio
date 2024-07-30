#ifndef __ETA_STUDIO__H__
#define __ETA_STUDIO__H__

#include <string>
#include <cstdint>
#include <fstream>
#include <vector>
#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include <SDL2/SDL_ttf.h>
#ifdef _WIN32
#include <imgui.h>
#elif defined(__linux__)
#include <imgui/imgui.h>
#endif
#include "imgui_sdl.hh"
#include <nlohmann/json.hpp>


#define xy(x, y) Vec({x, y}) // position macro
#define col(r, g, b, a) Color({r, g, b, a}) // colour macro RGBA
#define RGB(c) SDL_Color({c.r, c.g, c.b}) // convert color macro to RGB format
#define RGBA(c) SDL_Color({c.r, c.g, c.b, 255}) // convert color macro from RGB format to RGBA format

typedef struct vec {
    float x, y;
} Vec;

typedef struct color {
    uint8_t r;
    uint8_t g;
    uint8_t b;
    uint8_t a = 255;
} Color;

typedef struct screen {
    std::string title;
    SDL_Renderer* impl;
    SDL_Window* window;
    Vec dim = xy(800, 800);
    Vec offset;
    Color bg = { 0, 0, 0, 255 };
    bool fullscreen = false;
    bool done = false;
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
    Color c;
    bool fill = false;
} Box;

namespace EtaCore
{
    void init_sdl();
    void close_sdl(Screen*);
    void setup_screen(Screen*);
    void render_sdl(Screen*);
    void clear_sdl(Screen*);
    void update_sdl(Screen*);

    void handle_type(Screen*, int, void(*)());
    void handle_key(Screen*, int, void(*)());

    void draw_quad(Screen*, Vec, Vec, Color);
    void draw_circ(Screen*, Vec, float, Color);
    void draw_box_solid(Screen*, Vec, Vec, Color);
    void draw_box(Screen*, Box*);
    SDL_Texture* gen_text(Screen*, const char*, TTF_Font*, Color);
    void draw_texture(Screen*, SDL_Texture*, Vec);
    void draw_text(Screen*, Msg*);
    void draw_arrow(Screen*, Vec, Vec, Color);
    void draw_mesh(Screen*, Vec, Color);
    void draw_fillrect(Screen*, Vec, Vec, Color);

    Screen read_json(std::string);

    int eta_run(Screen*, void (*setup)(), void (*loop)());

    void message_box(const Screen*, const char*, const char*);
    void exit_with_error_msg(const Screen*, const char*);
    const char* get_error_msgs();
    void error_msg(const char*);
}

class Eta
{
public:
    Screen sc;

    std::vector<Msg> eta_msgs;
    std::vector<Box> eta_boxes;
    std::vector<TTF_Font*> eta_fonts;
    int mouseX, mouseY;

    float dt = 0.f;

    Eta(Screen m_sc) : sc(m_sc) {}
    ~Eta() {}

    void ClearFrame();
    void DrawMsgs();
    void DrawBoxes();
    void Setup();
    void Handle();
    void Loop();
    int Run();
};

#endif

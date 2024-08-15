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
#include <SDL2/SDL_image.h>
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
    bool operator==(const char arr[2])
    {
        if ((x == arr[0]) && (y == arr[1])) return true;
        return false;
    }
} Vec;

typedef struct color {
    uint8_t r;
    uint8_t g;
    uint8_t b;
    uint8_t a = 255;
} Color;

typedef struct screen {
    std::string title;
    SDL_Window* window;
    SDL_Renderer* impl;
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
    char* font;
    int size;
    Color c;
} Msg;

typedef struct tex {
    SDL_Surface* sur;
    SDL_Texture* tex;
    Vec pos;
} Tex;

typedef struct box {
    Msg* msgs;
    Tex* texs;
    Vec top, bottom;
    Color c;
    bool fill = false;
} Box;

typedef struct dynamic_entity {
    dynamic_entity(Vec m_pos, Vec m_size, const char* m_image=NULL, Color m_c=col(255, 255, 255, 255), float m_vel = 0.25);
    Screen* sc;
    float vel = 0.25;
    Vec size;
    Vec dir;
    Vec pos;
    bool move_right, move_left, move_down, move_up, is_jump;
    int jump_count = 10;
    Color c;
    const char* image;

    void Setup(Screen*);

    void SetVel(float nvel) { vel = nvel; }
    void SetPos(Vec npos) { pos = npos; }
    void SetDir(Vec ndir) { dir = ndir; }
    void SetCol(Color nc) { c = nc; }
    void SetImg(const char* nimage) { image = nimage; }

    void Draw();
private:
    SDL_Surface* sur;
    SDL_Texture* tex;
    SDL_Rect rect;
} DynamicEntity;

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
    //void draw_texture(Screen*, Tex, Vec, Vec);
    void draw_text(Screen*, Msg*);
    void draw_arrow(Screen*, Vec, Vec, Color);
    void draw_mesh(Screen*, Vec, Color);
    void draw_fillrect(Screen*, Vec, Vec, Color);
    //void draw_dynamic(Screen*, DynamicEntity);

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
    std::vector<DynamicEntity> eta_dynamic;
    int mouseX, mouseY;

    float dt = 0.f;

    Eta(Screen m_sc) : sc(m_sc) {}
    ~Eta() {}

    void ClearFrame();
    void DrawMsgs();
    void DrawBoxes();
    void Setup();
    //void SetupEntities();
    void Handle();
    void Loop();
    void Render();
    int Run();
};

#endif

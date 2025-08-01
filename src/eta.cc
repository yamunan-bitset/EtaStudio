#include "eta.hh"

// GENERAL:

void EtaCore::init_sdl()
{
    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_SetHint(SDL_HINT_VIDEO_X11_XRANDR, "1");
    SDL_SetHint(SDL_HINT_VIDEO_X11_NET_WM_BYPASS_COMPOSITOR, "0");
    TTF_Init();
    IMG_Init(IMG_INIT_JPG);
    IMG_Init(IMG_INIT_PNG);
    IMG_Init(IMG_INIT_TIF);
    IMG_Init(IMG_INIT_WEBP);
}

void EtaCore::close_sdl(Screen* sc)
{
    SDL_DestroyWindow(sc->window);
    TTF_Quit();
    SDL_Quit();
    IMG_Quit();
}

void EtaCore::setup_screen(Screen* sc)
{
    SDL_Window* window;
    SDL_Renderer* renderer;

    std::stringstream sst;
    sst << "EtaStudio: " << sc->title;
    window = SDL_CreateWindow(sst.str().c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
        sc->dim.x, sc->dim.y, sc->fullscreen ? SDL_WINDOW_FULLSCREEN : 0);

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    SDL_RenderSetLogicalSize(renderer, sc->dim.x - sc->offset.x, sc->dim.y - sc->offset.y);
    SDL_RenderClear(renderer);

    sc->impl = renderer;
    sc->window = window;
}

void EtaCore::render_sdl(Screen* sc)
{
    SDL_SetRenderDrawColor(sc->impl, sc->bg.r, sc->bg.g, sc->bg.b, sc->bg.a);
}

void EtaCore::clear_sdl(Screen* sc)
{
    SDL_RenderClear(sc->impl);
}

void EtaCore::update_sdl(Screen* sc)
{
    SDL_RenderPresent(sc->impl);
    SDL_UpdateWindowSurface(sc->window);
}

// HANDLE:
void EtaCore::handle_type(Screen* sc, int event, void (*fn)())
{
    if (sc->event.type == event) fn();
}

void EtaCore::handle_key(Screen* sc, int key, void (*fn)())
{
    if (sc->event.key.keysym.sym == key) fn();
}

// PRIMITIVES:

void EtaCore::draw_circ(Screen* sc, Vec p, float r, Color c)
{
    filledCircleRGBA(sc->impl, p.x, p.y, r, c.r, c.g, c.b, c.a);
}

void EtaCore::draw_box_solid(Screen* sc, Vec a, Vec b, Color c)
{
    boxRGBA(sc->impl, a.x, a.y, b.x, b.y, c.r, c.g, c.b, c.a);
}

void EtaCore::draw_box(Screen* sc, Box* bx)
{
    if (bx->fill) draw_fillrect(sc, bx->top, bx->bottom, bx->c);
    else rectangleRGBA(sc->impl, bx->top.x, bx->top.y, bx->bottom.x, bx->bottom.y, bx->c.r, bx->c.g, bx->c.b, bx->c.a);
    /*for (int i = 0; i < sizeof(bx->msgs) / sizeof(Msg); i++)
        draw_text(sc, &bx->msgs[i]);
    for (int i = 0; i < sizeof(bx->texs) / sizeof(Tex); i++)
        draw_texture(sc, bx->texs[i].tex, bx->texs[i].pos);*/
}

SDL_Texture* EtaCore::gen_text(Screen* sc, const char* text, TTF_Font* font, Color c)
{
    SDL_Surface* sur = TTF_RenderText_Solid(font, text, RGBA(c));
    SDL_Texture* tex = SDL_CreateTextureFromSurface(sc->impl, sur);
    SDL_FreeSurface(sur);
    return tex;
}

void EtaCore::draw_texture(Screen* sc, SDL_Texture* tex, Vec pos)
{
    SDL_Rect rect;
    rect.x = pos.x;
    rect.y = pos.y;
    SDL_QueryTexture(tex, NULL, NULL, &rect.w, &rect.h);
    SDL_RenderCopy(sc->impl, tex, NULL, &rect);
}

void EtaCore::draw_text(Screen* sc, Msg* msg)
{
    SDL_Texture* tex = EtaCore::gen_text(sc, msg->str, TTF_OpenFont(msg->font, msg->size), msg->c);
    EtaCore::draw_texture(sc, tex, msg->pos);
}

void EtaCore::draw_arrow(Screen* sc, Vec a, Vec b, Color c)
{
    SDL_SetRenderDrawColor(sc->impl, c.r, c.g, c.b, c.a);
    SDL_RenderDrawLine(sc->impl, a.x, a.y, b.x, b.y);
}

void EtaCore::draw_mesh(Screen* sc, Vec interval, Color c)
{
    SDL_SetRenderDrawColor(sc->impl, c.r, c.g, c.b, c.a);
    for (int i = 0; i <= sc->dim.y; i += interval.y)
        for (int j = 0; j <= sc->dim.x; j += interval.x)
        {
            SDL_RenderDrawLine(sc->impl, i, 0, i, sc->dim.y);
            SDL_RenderDrawLine(sc->impl, 0, j, sc->dim.y, j);
        }
}

void EtaCore::draw_fillrect(Screen* sc, Vec a, Vec b, Color c)
{
    boxRGBA(sc->impl, a.x, a.y, b.x, b.y, c.r, c.g, c.b, c.a);
}
/*
void EtaCore::draw_dynamic(Screen* sc, DynamicEntity de)
{
    if (de.image == NULL)
        EtaCore::draw_fillrect(sc, de.pos, xy(de.pos.x + de.size.x, de.pos.y + de.size.y), de.c);
    else
    {
        if (de.tex == NULL) printf("failed\n");
        SDL_RenderCopy(sc->impl, de.tex, NULL, &de.rect);
    }
}*/

Screen EtaCore::read_json(std::string file)
{
    std::ifstream f(file);
    nlohmann::json data = nlohmann::json::parse(f);
    std::string title = data.value("title", "title not found");
    std::string dim = data.value("dim", "dim not found");
    std::string bg = data.value("bg", "bg not found");
    size_t pos = dim.find('x');
    size_t bcol = bg.find(' ');
    int i = 0, ini = 0;
    uint8_t strs[4];
    while (bcol != std::string::npos)
    {
        strs[i] = std::stoi(bg.substr(ini, bcol - ini));
        ini = bcol + 1;
        bcol = bg.find(' ', ini);
        i++;
    }
    Screen sc(title, xy(std::stof(dim.substr(0, pos)), std::stof(dim.substr(pos + 1, dim.size() - 1))), col(strs[0], strs[1], strs[2], strs[3]));
    sc.done = false;
    return sc;
}

// ERRORS:
void EtaCore::message_box(const Screen* sc, const char* title, const char* message)
{
    SDL_ShowSimpleMessageBox(0, title, message, sc->window);
}

void EtaCore::exit_with_error_msg(const Screen* sc, const char* msg)
{
    EtaCore::message_box(sc, "EtaStudio:", msg);
    exit(1);
}

#define ERROR_BUF_SIZE 1024 * 32
static char error_msgs_buffer[ERROR_BUF_SIZE];

const char* EtaCore::get_error_msgs() { return error_msgs_buffer; }

void EtaCore::error_msg(const char* msg)
{
#ifdef _WIN32
    strncat_s(error_msgs_buffer, "\n", ERROR_BUF_SIZE - 1);
    strncat_s(error_msgs_buffer, msg, ERROR_BUF_SIZE - 1);
#elif defined(__linux__)
    strncat(error_msgs_buffer, "\n", ERROR_BUF_SIZE - 1);
    strncat(error_msgs_buffer, msg, ERROR_BUF_SIZE - 1);
#endif
}

dynamic_entity::dynamic_entity(Vec m_pos, Vec m_size, const char* m_image, Color m_c, float m_vel)
    : pos(m_pos), size(m_size), image(m_image), c(m_c), vel(m_vel)
{
}

void dynamic_entity::Setup(Screen* m_sc)
{
    sc = m_sc;
    IMG_Init(IMG_INIT_PNG);
    if (image != NULL)
    {
        rect = { (int)pos.x, (int)pos.y, (int)size.x, (int)size.y };
        sur = IMG_Load(image);
        tex = SDL_CreateTextureFromSurface(sc->impl, sur);
        SDL_FreeSurface(sur);
    }
}

void dynamic_entity::Draw()
{
    if (image == NULL)
        EtaCore::draw_fillrect(sc, pos, xy(pos.x + size.x, pos.y + size.y), c);
    else
    {
        rect = { (int)pos.x, (int)pos.y, (int)size.x, (int)size.y };
        SDL_RenderCopy(sc->impl, tex, NULL, &rect);
    }
}

void Eta::DrawMsgs()
{
    for (Msg u : eta_msgs)
        EtaCore::draw_text(&sc, &u);
}

void Eta::DrawBoxes()
{
    for (Box u : eta_boxes)
        EtaCore::draw_box(&sc, &u);
}
/*
void Eta::SetupEntities()
{
    for (DynamicEntity u : eta_dynamic)
        u.Setup(&sc);
}
*/
void Eta::ClearFrame()
{
    EtaCore::clear_sdl(&sc);
}

int Eta::Run()
{
    EtaCore::init_sdl();
    EtaCore::setup_screen(&sc);
    ImGui::CreateContext();
    ImGuiSDL::Initialize(sc.impl, sc.dim.x, sc.dim.y);
    Setup();
    //SetupEntities();
    dt = 0;
    Uint64 NOW = SDL_GetPerformanceCounter();
    Uint64 LAST = 0;
    while (!sc.done)
    {
        LAST = NOW;
        NOW = SDL_GetPerformanceCounter();
        dt = (double)((NOW - LAST) * 1000 / (double)SDL_GetPerformanceFrequency());
        const int buttons = SDL_GetMouseState(&mouseX, &mouseY);
        int wheel = 0;
        wheel = sc.event.wheel.y;
        ImGuiIO& io = ImGui::GetIO();
        io.DisplaySize = ImVec2(sc.dim.x, sc.dim.y);
        io.DeltaTime = 1.0f / 60.0f;
        io.MousePos = ImVec2(static_cast<float>(mouseX), static_cast<float>(mouseY));
        io.MouseDown[0] = buttons & SDL_BUTTON(SDL_BUTTON_LEFT);
        io.MouseDown[1] = buttons & SDL_BUTTON(SDL_BUTTON_RIGHT);
        io.MouseWheel = static_cast<float>(wheel);
        EtaCore::render_sdl(&sc);
        ImGui::NewFrame();
        Handle();
        Loop();
        ClearFrame();
        DrawBoxes();
        DrawMsgs();
        Render();
        ImGui::Render();
        ImGuiSDL::Render(ImGui::GetDrawData());
        EtaCore::update_sdl(&sc);
    }

    ImGuiSDL::Deinitialize();
    EtaCore::close_sdl(&sc);
    return 0; // TODO: Test For Failures
}

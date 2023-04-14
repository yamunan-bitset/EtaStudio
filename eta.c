#include "eta.h"

// GENERAL:

void init_sdl()
{
  SDL_Init(SDL_INIT_VIDEO);
  TTF_Init();
}

void close_sdl(Screen* sc)
{
  SDL_DestroyWindow(sc->window);
  TTF_Quit();
  SDL_Quit();
}

void setup_screen(Screen *sc)
{
  SDL_Window* window;
  SDL_Renderer* renderer;

  window = SDL_CreateWindow("EtaStudio", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
			    sc->dim.x, sc->dim.y, sc->fullscreen ? SDL_WINDOW_FULLSCREEN : 0);

  renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
  SDL_RenderSetLogicalSize(renderer, sc->dim.x-sc->offset.x, sc->dim.y-sc->offset.y);
  SDL_RenderClear(renderer);

  sc->impl = renderer;
  sc->window = window;
}

void render_sdl(Screen *sc)
{
  SDL_SetRenderDrawColor(sc->impl, sc->bg.r, sc->bg.g, sc->bg.b, sc->bg.a);
  SDL_RenderClear(sc->impl);
}

void update_sdl(Screen *sc)
{
  SDL_RenderPresent(sc->impl);
  SDL_UpdateWindowSurface(sc->window);
}

// HANDLE:
void handle_type(Screen *sc, int event, void (*fn)())
{
  if (sc->event.type == event) fn();
}

void handle_key(Screen *sc, int key, void (*fn)())
{
  if (sc->event.key.keysym.sym == key) fn();
}

// PRIMITIVES:

SDL_Rect *gen_rect(Vec a, Vec b)
{
  return &(SDL_Rect) { a.x, a.y, b.x, b.y };
}

void draw_quad(Screen *sc, Vec a, Vec b, Color c)
{
  SDL_SetRenderDrawColor(sc->impl, c.r, c.g, c.b, c.a);
  SDL_RenderFillRect(sc->impl, gen_rect(a, b));
}

void draw_box(Screen *sc, Vec a, Vec b, Color c)
{
  boxRGBA(sc->impl, a.x, b.x, a.y, b.y, c.r, c.g, c.b, c.a);
}

void draw_circ(Screen *sc, Vec p, float r, Color c)
{
  filledCircleRGBA(sc->impl, p.x, p.y, r, c.r, c.g, c.b, c.a);
}

void draw_text(Screen *sc, const char *text, const char *font, int size, Vec a, Vec b, Color c)
{
  TTF_Font* f = TTF_OpenFont(font, size);
  SDL_Surface* s_m = TTF_RenderText_Solid(f, text, RGB(c));
  SDL_Texture* msg = SDL_CreateTextureFromSurface(sc->impl, s_m);
  SDL_RenderCopy(sc->impl, msg, NULL, gen_rect(a, b));
  SDL_FreeSurface(s_m);
  SDL_DestroyTexture(msg);
  TTF_CloseFont(font);
}

// ERRORS:
void message_box(const Screen *sc, const char *title, const char *message)
{
  SDL_ShowSimpleMessageBox(0, title, message, sc->window);
}

void exit_with_error_msg(const Screen* sc, const char* msg)
{
  message_box("EtaStudio:", msg, sc);
  exit(1);
}

#define ERROR_BUF_SIZE 1024 * 32
static char error_msgs_buffer[ERROR_BUF_SIZE];

const char *get_error_msgs()  { return error_msgs_buffer; }

void error_msg(const char* msg)
{
  strncat(error_msgs_buffer, "\n", ERROR_BUF_SIZE - 1);
  strncat(error_msgs_buffer, msg, ERROR_BUF_SIZE - 1);
}
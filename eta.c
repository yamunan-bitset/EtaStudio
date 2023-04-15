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

void draw_circ(Screen *sc, Vec p, float r, Color c)
{
  filledCircleRGBA(sc->impl, p.x, p.y, r, c.r, c.g, c.b, c.a);
}

void draw_box_solid(Screen *sc, Vec a, Vec b, Color c)
{
  boxRGBA(sc->impl, a.x, a.y, b.x, b.y, c.r, c.g, c.b, c.a);
}

void draw_box(Screen *sc, Box *bx, Color c)
{
  rectangleRGBA(sc->impl, bx->top.x, bx->top.y, bx->bottom.x, bx->bottom.y, c.r, c.g, c.b, c.a);
  for (int i = 0; i < sizeof(bx->msgs) / sizeof(Msg); i++)
    draw_text(&sc, &bx->msgs[i]);
  for (int i = 0; i < sizeof(bx->texs) / sizeof(Tex); i++)
    draw_texture(&sc, bx->texs[i].tex, bx->texs[i].pos);
}

SDL_Texture *gen_text(Screen *sc, const char *text, TTF_Font *font, Color c)
{
  SDL_Surface *sur = TTF_RenderText_Solid(font, text, RGBA(c));
  SDL_Texture *tex = SDL_CreateTextureFromSurface(sc->impl, sur);
  SDL_FreeSurface(sur);
  return tex;
}

void draw_texture(Screen *sc, SDL_Texture *tex, Vec a)
{
  SDL_Rect rect;
  rect.x = a.x;
  rect.y = a.y;
  SDL_QueryTexture(tex, NULL, NULL, &rect.w, &rect.h);
  SDL_RenderCopy(sc->impl, tex, NULL, &rect);
}

void draw_text(Screen *sc, Msg *msg)
{
  SDL_Texture* tex = gen_text(&sc, msg->str, msg->font, msg->c);
  draw_texture(&sc, tex, msg->pos);
}

// ERRORS:
void message_box(const Screen *sc, const char *title, const char *message)
{
  SDL_ShowSimpleMessageBox(0, title, message, sc->window);
}

void exit_with_error_msg(const Screen* sc, const char* msg)
{
  message_box(sc, "EtaStudio:", msg);
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

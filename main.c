#include "eta.h"

// Example file for 3body simulator

static Screen sc = {
  .dim = xy(1400, 900),
  .offset = xy(20, 20),
  .bg = { 2, 2, 10, 255 },
  .fullscreen = false,
  .done = false
};

int main()
{
  // Setup
  init_sdl();
  setup_screen(&sc);

  // Font
  TTF_Font *font = TTF_OpenFont("TheSansPlain.ttf", 20);

  Vec a = xy(100, 100);
  Vec b = xy(300, 300);
  
  // Loop
  while (!sc.done)
    {
      // Handle:
      while (SDL_PollEvent(&sc.event))
	{ 
	  switch (sc.event.type)
	    {
	    case SDL_QUIT:
	      sc.done = true;
	      break;
	    }
	  switch (sc.event.key.keysym.sym)
	    {
	    case SDLK_q:
	      sc.done = true;
	      break;
	    case SDLK_x:
	      message_box(&sc, "Hello, World!", "Hi");
	      break;
		  
	    case SDLK_w:
	      a.y -= 100;
	      break;
	    case SDLK_a:
	      a.x -= 100;
	      break;
	    case SDLK_s:
	      a.y += 100;
	      break;
	    case SDLK_d:
	      a.x += 100;
	      break;

	    case SDLK_UP:
	      b.y -= 100;
	      break;
	    case SDLK_LEFT:
	      b.x -= 100;
	      break;
	    case SDLK_DOWN:
	      b.y += 100;
	      break;
	    case SDLK_RIGHT:
	      b.x += 100;
	      break;
	    }
	}

      // Render:
      render_sdl(&sc);
      draw_circ(&sc, xy(sc.dim.x / 2, sc.dim.y / 2), 50, col(200, 200, 30, 255));
      draw_box(&sc, a, b, col(100, 0, 0, 255));
      draw_box(&sc, xy(1100, 700), xy(1300, 900), col(255, 0, 0, 255));
      draw_text(&sc, "Hello, World! From Eta!", font, xy(100, 100), col(255, 0, 0, 255));
      update_sdl(&sc);
    }
  // Quit:
  close_sdl(&sc);
  return 0;
}

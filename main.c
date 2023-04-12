#include "eta.h"

static Screen sc = {
  .dim = xy(1400, 900),
  .offset = xy(20, 20),
  .bg = { 10, 10, 40, 255 },
  .fullscreen = false,
  .done = false
};

int main()
{
  // Setup
  init_sdl();
  setup_screen(&sc);
  // Loop
  while (!sc.done)
    {
      // Handle:
      if (SDL_PollEvent(&sc.event))
	{ 
	  switch (sc.event.type)
	    {
	    case SDL_QUIT:
	      sc.done = true;
	      break;
	    case SDL_KEYDOWN:
	      switch (sc.event.key.keysym.sym)
		{
		case SDLK_q:
		  sc.done = true;
		  break;
		case SDLK_x:
		  message_box(&sc, "Hello, World!", "Hi");
		  break;
		}
	    }
	  // Render:
	  render_sdl(&sc);
	  draw_quad(&sc, xy(20, 80), xy(40, 60), col(10, 255, 10, 200));
	  draw_circ(&sc, xy(sc.dim.x / 2, sc.dim.y / 2), 50, col(200, 10, 30, 100));
	  draw_box(&sc, xy(750, 480), xy(500, 700), col(0, 0, 0, 255));
	  update_sdl(&sc);
	}
    }
  // Quit:
  close_sdl(&sc);
  return 0;
}

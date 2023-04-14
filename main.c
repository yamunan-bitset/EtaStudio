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
  SDL_Texture *help = gen_text(&sc, "Press h for Help", font, col(0, 255, 0, 255));
  SDL_Texture *start = gen_text(&sc, "Press space to start sim", font, col(0, 255, 0, 255));
  
  // 3 bodies:
  Vec b1, b2, b3;

  // Loop
  int mouseX, mouseY;
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
	    case SDL_KEYDOWN:
	      switch (sc.event.key.keysym.sym)
		{
		case SDLK_q:
		  sc.done = true;
		  break;
		case SDLK_h:
		  message_box(&sc, "Help", "Click 3 times to spawn 3 bodies.\nThen press space to start.");
		  break;
		case SDLK_SPACE:
		  if (b1.x == 0 && b1.y == 0
		      || b2.x == 0 && b2.y == 0
		      || b3.x == 0 && b3.y == 0)
		    message_box(&sc, "Error", "Cannot start sim.\n Are you sure you clicked 3 times?\nPress h for more details.");
		  // TODO: start sim
		  break;
		}
	      break;
	    case SDL_MOUSEBUTTONDOWN:
	      SDL_GetMouseState(&mouseX, &mouseY);
	      switch (sc.event.button.button)
		{
		case SDL_BUTTON_LEFT:
		  if (b1.x == 0 && b1.y == 0)
		    {
		      b1.x = mouseX;
		      b1.y = mouseY;
		    }
		  else if (b2.x == 0 && b2.y == 0)
		    {
		      b2.x = mouseX;
		      b2.y = mouseY;
		    }
		  else if (b3.x == 0 && b3.y == 0)
		    {
		      b3.x = mouseX;
		      b3.y = mouseY;
		    }
		  break;
		}
	      break;  
	    }
	}

      // Render:
      render_sdl(&sc);

      if (b1.x != 0 && b1.y != 0)
	draw_circ(&sc, b1, 30, col(200, 200, 30, 255));
      if (b2.x != 0 && b2.y != 0)
	draw_circ(&sc, b2, 30, col(30, 200, 200, 255));
      if (b3.x != 0 && b3.y != 0)
	draw_circ(&sc, b3, 30, col(200, 30, 200, 255));
      
      draw_box(&sc, xy(1100, 700), xy(1300, 900), col(255, 0, 0, 255));
      draw_texture(&sc, help, xy(1130, 825));
      draw_texture(&sc, start, xy(1100, 850));
      update_sdl(&sc);
    }
  // Quit:
  close_sdl(&sc);
  return 0;
}

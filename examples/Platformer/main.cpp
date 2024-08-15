#include "ETA/eta.hh"

Screen sc = {
	.title = "Basic",
	.dim = xy(1200, 800)
};

Box platforms[3] = {
	{
		.top = xy(100, 600),
		.bottom = xy(400, 620),
		.c = col(0, 0, 255, 255),
		.fill = true
	},
	{
		.top = xy(800, 600),
		.bottom = xy(1100, 620),
		.c = col(0, 0, 255, 255),
		.fill = true
	},
	{
		.top = xy(300, 450),
		.bottom = xy(900, 470),
		.c = col(0, 0, 255, 255),
		.fill = true
	}
};

DynamicEntity player(xy(0, sc.dim.y - player.size.y), xy(32, 32), "sprite.png", col(255, 255, 255, 255), 0.5);

void Eta::Setup() 
{
	player.Setup(&sc);
	for (Box platform : platforms)
		eta_boxes.push_back(platform);
	eta_dynamic.push_back(player);
}

void Eta::Handle() 
{
	while (SDL_PollEvent(&sc.event))
	{
		switch (sc.event.type)
		{
		case SDL_QUIT: sc.done = true; break;
		case SDL_KEYDOWN:
			switch (sc.event.key.keysym.sym)
			{
			case SDLK_LEFT: player.move_left = true; break;
			case SDLK_RIGHT: player.move_right = true; break;
			case SDLK_DOWN: player.move_down = true; break; 
			default: break;
			} break;
		case SDL_KEYUP: 
			switch (sc.event.key.keysym.sym)
			{
			case SDLK_LEFT: player.move_left = false; break;
			case SDLK_RIGHT: player.move_right = false; break;
			case SDLK_DOWN: player.move_down = false; break; 
			case SDLK_SPACE: player.move_up = true; break;
			default: break;
			} break;
		default: break;
		}	
	}
};

int i = 0;
bool on_platform1 = false;
bool on_platform2 = false;
bool falling = false;
bool falling_from2 = false;
void Eta::Loop() 
{
	i++;
	// key events update position
	if (player.move_right) player.pos.x += player.vel * dt;
	if (player.move_left) player.pos.x -= player.vel * dt;
	if (player.move_down) 
	{ 
		on_platform1 = false; 
		on_platform2 = false;  
		player.is_jump = false; 
	}
	if (player.move_up) 
	{
		player.jump_count = -20;  
		player.is_jump = true; 
		player.move_up = false; 
	}

	// player jump
	if (player.is_jump)
	{
		if (i % 10 == 0) player.jump_count += 1;
		player.pos.y += player.jump_count * dt * 0.1;
	}

	// player and platform
	if ((player.is_jump && player.jump_count > 0) || falling)
	{
		if ((564 < player.pos.y && player.pos.y < 570) && ((player.pos.x > 100 && player.pos.x < 400) || (player.pos.x > 800 && player.pos.x < 1100)))
		{
			player.pos.y = 568;
			player.is_jump = false;
			on_platform1 = true;
			on_platform2 = false;
			falling = false;
		}
		else if ((416 < player.pos.y && player.pos.y < 420) && (player.pos.x > 300 && player.pos.x < 900))
		{
			player.pos.y = 418;
			player.is_jump = false;
			on_platform2 = true;
			on_platform1 = false;
			falling = false;
		}
	}
	if (on_platform1 && !((player.pos.x > 100 && player.pos.x < 400) || (player.pos.x > 800 && player.pos.x < 1100))) on_platform1 = false;
	if (on_platform2 && !(player.pos.x > 300 && player.pos.x < 900)) on_platform2 = false;
	if (!on_platform1 && !on_platform2 && !player.is_jump)
	{
		falling = true;
		player.pos.y += 5; // add gravity if not on a platform
	}

	// clamp player to window
	if (player.pos.x > sc.dim.x - 32) { player.pos.x = sc.dim.x - 32; }
	if (player.pos.x < 0) player.pos.x = 0;
	if (player.pos.y > sc.dim.y - 32) { player.pos.y = sc.dim.y - 32; player.is_jump = false; }
	if (player.pos.y < 0) { player.pos.y = 0; player.move_up = false; }
}

void Eta::Render()
{
	//player.tex.pos = player.pos;
	//EtaCore::draw_dynamic(&sc, player);
	//SDL_RenderCopy(sc.impl, lettuce_tex, NULL, &bRect);
	player.Draw();
}

int main()
{
	Eta eta(sc);
	return eta.Run();
}/*


#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#define WIDTH 800
#define HEIGHT 600
#define IMG_PATH "../sprite.png"
#include <iostream>

int main(int argc, char* argv[]) {

	printf(SDL_GetBasePath(), "%s");
	// variable declarations
	SDL_Window* win = NULL;
	SDL_Renderer* renderer = NULL;
	SDL_Texture* img = NULL;
	int w, h; // texture width & height

	// Initialize SDL.
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
		return 1;

	// create the window and renderer
	// note that the renderer is accelerated
	win = SDL_CreateWindow("Image Loading", 100, 100, WIDTH, HEIGHT, 0);
	renderer = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);

	SDL_Surface* lettuce_sur = IMG_Load("sprite.png");
	if (lettuce_sur == NULL) {
		std::cout << "Error loading image: " << IMG_GetError();
		return 5;
	}
	SDL_Texture* lettuce_tex = SDL_CreateTextureFromSurface(renderer, lettuce_sur);
	if (lettuce_tex == NULL) {
		std::cout << "Error creating texture";
		return 6;
	}

	SDL_FreeSurface(lettuce_sur);

	// main loop
	while (1) {

		// event handling
		SDL_Event e;
		if (SDL_PollEvent(&e)) {
			if (e.type == SDL_QUIT)
				break;
			else if (e.type == SDL_KEYUP && e.key.keysym.sym == SDLK_ESCAPE)
				break;
		}

		SDL_RenderClear(renderer);
		SDL_RenderCopy(renderer, lettuce_tex, NULL, NULL);
		SDL_RenderPresent(renderer);
	}

	SDL_DestroyTexture(img);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(win);

	return 0;
}*/
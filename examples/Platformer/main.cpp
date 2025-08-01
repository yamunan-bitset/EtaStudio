#include "ETA/eta.hh"

Screen sc = {
	.title = "Basic",
	.dim = xy(1200, 800),
	.bg = col(255, 255, 255, 255)
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

int size = 162;
DynamicEntity player(xy(0, sc.dim.y - player.size.y), xy((float) size, (float) size), "Attack1.png", col(255, 255, 255, 255), 0.5);

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

SDL_Rect size_rect;
int i = 0, state = 0;
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
		if ((598 - size < player.pos.y && player.pos.y < 602 - size) && ((player.pos.x > 100 && player.pos.x < 400) || (player.pos.x > 800 && player.pos.x < 1100)))
		{
			player.pos.y = 600 - size;
			player.is_jump = false;
			on_platform1 = true;
			on_platform2 = false;
			falling = false;
		}
		else if ((448 - size < player.pos.y && player.pos.y < 452 - size) && (player.pos.x > 300 && player.pos.x < 900))
		{
			player.pos.y = 450 - size;
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
	if (player.pos.x > sc.dim.x - size) { player.pos.x = sc.dim.x - size; }
	if (player.pos.x < 0) player.pos.x = 0;
	if (player.pos.y > sc.dim.y - size) { player.pos.y = sc.dim.y - size; player.is_jump = false; }
	if (player.pos.y < 0) { player.pos.y = 0; player.move_up = false; }

	// Idle state
	if (dt == 0.5) state++;
	if (i % 200 == 0)
		size_rect = { 162, 0, 324, 162 };
	else
		size_rect = { 0, 0, 162, 162 };
}

void Eta::Render()
{
	player.Draw(size_rect);
}

int main()
{
	Eta eta(sc);
	return eta.Run();
}
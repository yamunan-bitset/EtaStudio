#include "ETA/eta.hh"

Screen sc = {
	.title = "Basic",
	.dim = xy(1000, 700)
};

Box p1 = {
	.top = xy(100, 600),
	.bottom = xy(400, 620),
	.c = col(0, 0, 255, 255),
	.fill = true
};

Box p2 = {
	.top = xy(600, 600),
	.bottom = xy(900, 620),
	.c = col(0, 0, 255, 255),
	.fill = true
};

Box p3 = {
	.top = xy(300, 100),
	.bottom = xy(700, 120),
	.c = col(0, 0, 255, 255),
	.fill = true
};

StaticEntity platforms[3] = {
	{
		.pos = xy(300, 100),
		.sizex = 400,
		.sizey = 20
	},
	{
		.pos = xy(100, 600),
		.sizex = 300,
		.sizey = 20
	}, 
	{
		.pos = xy(600, 600),
		.sizex = 300,
		.sizey = 20
	}
};

DynamicEntity player = {
	.vel = 0.25,
	.size = 32,
	.pos = xy(0, sc.dim.y - player.size),
	.jump_count = 10,
	.c = col(255, 0, 0, 255)
};

bool keydown = false;

void Eta::Setup() 
{
	eta_boxes.push_back(p1);
	eta_boxes.push_back(p2);
	eta_boxes.push_back(p3);
};

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
			case SDLK_SPACE: player.move_up = true; break; 
			default: break;
			} break;
		case SDL_KEYUP: 
			switch (sc.event.key.keysym.sym)
			{
			case SDLK_LEFT: player.move_left = false; break;
			case SDLK_RIGHT: player.move_right = false; break;
			case SDLK_DOWN: player.move_down = false; break; 
			case SDLK_SPACE: player.move_up = false; break;
			default: break;
			} break;
		default: break;
		}	
	}
};

void Eta::Loop() 
{
	if (player.move_right) player.dir.x += 1;
	if (player.move_left) player.dir.x -= 1;
	if (player.move_down) player.dir.y += 1;
	if (player.move_up) {  player.is_jump = true; }
	
	if (player.is_jump)
	{
		if (player.jump_count >= -10)
		{
			int neg = 1;
			if (player.jump_count < 0) neg = -1;
			player.pos.y -= (player.jump_count * player.jump_count) * 2 * neg * player.vel;
			player.jump_count -= 1;
		}
		else 
		{ 
			player.is_jump = false; 
			player.jump_count = 10; 
		}
	}

	player.pos = xy(player.pos.x + player.dir.x * player.vel, player.pos.y + player.dir.y * player.vel);
	player.dir = xy(0, 0);

	if (player.pos.x > sc.dim.x - 32) player.pos.x = sc.dim.x - 32;
	if (player.pos.x < 0) player.pos.x = 0;
	if (player.pos.y > sc.dim.y - 32) player.pos.y = sc.dim.y - 32;
	if (player.pos.y < 0) { player.pos.y = 0; player.move_up = false; }

	ClearFrame();
	EtaCore::draw_fillrect(&sc, player.pos, xy(player.pos.x + 32, player.pos.y + 32), col(255, 0, 0, 255));
};

int main()
{
	Eta eta(sc);
	return eta.Run();
}
#include "ETA/eta.hh"

Screen sc = {
	.title = "Basic",
	.dim = xy(1200, 800)
};

Box p1 = {
	.top = xy(100, 600),
	.bottom = xy(400, 620),
	.c = col(0, 0, 255, 255),
	.fill = true
};

Box p2 = {
	.top = xy(800, 600),
	.bottom = xy(1100, 620),
	.c = col(0, 0, 255, 255),
	.fill = true
};

Box p3 = {
	.top = xy(300, 450),
	.bottom = xy(900, 470),
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
	.jump_count = 100,
	.c = col(255, 0, 0, 255)
};

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

void Eta::Loop() 
{
	i++;
	if (player.move_right) player.pos.x += player.vel * dt;
	if (player.move_left) player.pos.x -= player.vel * dt;
	if (player.move_down) player.pos.y += player.vel * dt;

	if (player.move_up) { player.jump_count = -20;  player.is_jump = true; player.move_up = false; }

	if (player.is_jump)
	{
		if (i % 10 == 0) player.jump_count += 1;
		player.pos.y += player.jump_count * dt * 0.1;
	}
	else player.jump_count = 0;

	if (player.pos.x > sc.dim.x - 32) { player.pos.x = sc.dim.x - 32; }
	if (player.pos.x < 0) player.pos.x = 0;
	if (player.pos.y > sc.dim.y - 32) { player.pos.y = sc.dim.y - 32; player.is_jump = false; }
	if (player.pos.y < 0) { player.pos.y = 0; player.move_up = false; }
}

void Eta::Render()
{
	EtaCore::draw_fillrect(&sc, player.pos, xy(player.pos.x + 32, player.pos.y + 32), col(255, 0, 0, 255));
}

int main()
{
	Eta eta(sc);
	return eta.Run();
}

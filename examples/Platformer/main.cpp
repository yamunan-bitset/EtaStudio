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

class Player
{
public:
	float vel = 0.25;
	Vec dir;
	Vec pos = xy(0, 690);
	bool move_right, move_left, move_down, move_up, is_jump;
	int jump_count = 10;
} p;

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
			case SDLK_LEFT: p.move_left = true; break;
			case SDLK_RIGHT: p.move_right = true; break;
			case SDLK_DOWN: p.move_down = true; break; 
			case SDLK_SPACE: p.move_up = true; break; 
			default: break;
			} break;
		case SDL_KEYUP: 
			switch (sc.event.key.keysym.sym)
			{
			case SDLK_LEFT: p.move_left = false; break;
			case SDLK_RIGHT: p.move_right = false; break;
			case SDLK_DOWN: p.move_down = false; break; 
			case SDLK_SPACE: p.move_up = false; break;
			default: break;
			} break;
		default: break;
		}	
	}
};

void Eta::Loop() 
{
	if (p.move_right) p.dir.x += 1;
	if (p.move_left) p.dir.x -= 1;
	if (p.move_down) p.dir.y += 1;
	if (p.move_up) {  p.is_jump = true; }
	
	if (p.is_jump)
	{
		if (p.jump_count >= -10)
		{
			int neg = 1;
			if (p.jump_count < 0) neg = -1;
			p.pos.y -= (p.jump_count * p.jump_count) * 0.5 * neg * p.vel;
			p.jump_count -= 1;
		}
		else 
		{ 
			p.is_jump = false; 
			p.jump_count = 10; 
		}
	}

	p.pos = xy(p.pos.x + p.dir.x * p.vel, p.pos.y + p.dir.y * p.vel);
	p.dir = xy(0, 0);

	if (p.pos.x > sc.dim.x - 10) p.pos.x = sc.dim.x - 10;
	if (p.pos.x < 0) p.pos.x = 0;
	if (p.pos.y > sc.dim.y - 10) p.pos.y = sc.dim.y - 10;
	if (p.pos.y < 0) { p.pos.y = 0; p.move_up = false; }

	ClearFrame();
	EtaCore::draw_fillrect(&sc, p.pos, xy(p.pos.x + 10, p.pos.y + 10), col(255, 0, 0, 255));
};

int main()
{
	Eta eta(sc);
	return eta.Run();
}
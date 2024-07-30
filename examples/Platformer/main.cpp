#include "ETA/eta.hh"

Screen sc = {
	.title = "Basic",
	.dim = xy(1000, 700)
};

Box p1 = {
	.top = xy(100, 600),
	.bottom = xy(400, 620),
	.c = col(0, 0, 255, 255)
};

Box p2 = {
	.top = xy(600, 600),
	.bottom = xy(900, 620),
	.c = col(0, 0, 255, 255)
};

Box p3 = {
	.top = xy(300, 100),
	.bottom = xy(700, 120),
	.c = col(0, 0, 255, 255)
};

class Player
{
public:
	float vel = 8;
	Vec dir;
	Vec pos = xy(50., 50.);
} p;

bool keydown = false;

void Eta::Setup() 
{
	boxes.push_back(p1);
	boxes.push_back(p2);
	boxes.push_back(p3);
};

void Eta::Handle() 
{
	const Uint8* keystates = SDL_GetKeyboardState(NULL);

	while (SDL_PollEvent(&sc.event))
	{
		switch (sc.event.type)
		{
		case SDL_QUIT: sc.done = true;
		case SDL_KEYDOWN: 
			switch (sc.event.key.keysym.sym)
			{
			case SDLK_LEFT: p.dir = xy(-1, 0); printf("Left\n"); break;
			case SDLK_RIGHT: p.dir = xy(1, 0); printf("Right\n"); break;
			case SDLK_DOWN: p.dir = xy(0, 1); printf("Down\n"); break;
			case SDLK_UP: p.dir = xy(0, -1); printf("Up\n"); break;
			default: break;
			} break;
		default: break;
		}	
	}
};

void Eta::Loop() 
{
	p.pos = xy(p.pos.x + p.dir.x * p.vel, p.pos.y + p.dir.y * p.vel);
	p.dir = xy(0, 0);
	ClearFrame();
	EtaCore::draw_fillrect(&sc, p.pos, xy(p.pos.x + 10, p.pos.y + 10), col(255, 0, 0, 255));
};

int main()
{
	Eta eta(sc);
	return eta.Run();
}
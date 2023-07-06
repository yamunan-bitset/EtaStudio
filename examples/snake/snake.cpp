#include "../../src/eta.hh"

Screen sc = {
	.title = "Snake",
	.dim = xy(900, 900),
	.bg = col(0, 0, 0, 255),
};

struct Player {
	int vel;
	Vec head = xy(100, 100);
	int length;
	Vec dir = xy(0, 0);
} p;

void Eta::Setup()
{
	draw_mesh(&sc, xy(50, 50), col(255, 0, 0, 255));
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
			case SDLK_LEFT: p.dir = xy(-1, 0); break;
			case SDLK_RIGHT: p.dir = xy(1, 0); break;
			case SDLK_DOWN: p.dir = xy(0, 1); break;
			case SDLK_UP: p.dir = xy(0, -1); break;
			default: break;
			}
		default: break;
		}
	}
}

void Eta::Loop()
{
	p.head.x += p.dir.x * 50;
	p.head.y += p.dir.y * 50;
	draw_fillrect(&sc, p.head, xy(p.head.x + 50, p.head.y + 50), col(0, 255, 0, 255));
	p.dir = xy(0, 0);
}

int main()
{
	Eta eta(sc);
	return eta.Run();
}

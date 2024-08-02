#include "../../src/eta.hh"

Screen sc = {
	.title = "Snake",
	.dim = xy(900, 900),
	.bg = col(0, 0, 0, 255),
};

int itime;

class Player {
public:
	int vel;
	Vec head;
	int length;
	Vec dir;
} p;

class Fruit {
public:
	Vec pos;
	bool taken;
	int iter;
} f;

void Eta::Setup()
{
	std::srand(std::time(NULL));
	p.head = xy(100, 100);
	p.length = 2;
	p.dir = xy(0, 1);
	int x = 50 * (std::rand() % 10);
	int y = 50 * (std::rand() % 10);
	f.pos = xy(x, y);
	f.taken = false;
	f.iter = 1;
	itime = 0;
}
#include <stdio.h>
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
			case SDLK_LEFT: p.dir = xy(-1, 0); printf("Left\n"); break;
			case SDLK_RIGHT: p.dir = xy(1, 0); printf("Right\n"); break;
			case SDLK_DOWN: p.dir = xy(0, 1); printf("Down\n"); break;
			case SDLK_UP: p.dir = xy(0, -1); printf("Up\n"); break;
			default: break;
			}
		default: break;
		}
	}
}

void Eta::Loop()
{
	itime++;
	if (itime % 100 == 0)
	{
		p.head.x += p.dir.x * 50;
		p.head.y += p.dir.y * 50;
	}
	if (p.head.x > 900)
		p.head.x = 0;
	if (p.head.y > 900)
		p.head.y = 0;
	if (p.head.x < 0)
		p.head.x = 900;
	if (p.head.y < 0)
		p.head.y = 900;
	if (p.head.x == f.pos.x && p.head.y == f.pos.y)
		f.taken = true;
}

void Eta::Render()
{
	EtaCore::draw_mesh(&sc, xy(50, 50), col(255, 0, 0, 255));
	for (int i = 0; i < p.length; i++)
		EtaCore::draw_fillrect(&sc, p.head, xy(p.head.x + 50 + 50 * i * std::abs(p.dir.x), p.head.y + 50 + 50 * i * std::abs(p.dir.y)), col(0, 255, 0, 255));
	if (!f.taken)
		EtaCore::draw_fillrect(&sc, f.pos, xy(f.pos.x + 50, f.pos.y + 50), col(255, 0, 0, 255));
	else
	{
		f.taken = false;
		int x = 50 * (std::rand() % 10);
		int y = 50 * (std::rand() % 10);
		f.pos = xy(x, y);
		p.length++;
	}
}

int main()
{
	Eta eta(sc);
	return eta.Run();
}

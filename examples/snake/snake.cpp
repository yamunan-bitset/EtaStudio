#include "../../src/eta.hh"
#include <vector>

Screen sc("Snake", xy(900, 900), col(0, 0, 0, 255));

int itime;

class Player {
public:
	int vel;
	int length;
	Vec dir;
	std::vector<Vec> body;
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
	p.body.push_back(xy(100, 100));
	p.body.push_back(xy(100, 50));
	p.length = 2;
	p.dir = xy(0, 1);
	int x = 50 * (std::rand() % 10);
	int y = 50 * (std::rand() % 10);
	f.pos = xy(x, y);
	f.taken = false;
	f.iter = 1;
	itime = 0;
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
			case SDLK_UP: p.dir = xy(0, -1);  break;
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
		Vec new_head_pos = xy(p.body[0].x + p.dir.x * 50, p.body[0].y + p.dir.y * 50);

		if (new_head_pos.x > 900)
			new_head_pos.x = 0;
		if (new_head_pos.y > 900)
			new_head_pos.y = 0;
		if (new_head_pos.x < 0)
			new_head_pos.x = 900;
		if (new_head_pos.y < 0)
			new_head_pos.y = 900;

		p.body.insert(p.body.begin(), new_head_pos);

		if (p.body[0].x == f.pos.x && p.body[0].y == f.pos.y)
			f.taken = true;
		else if (p.body.size() > p.length)
			p.body.pop_back();
	}
}

void Eta::Render()
{
	EtaCore::draw_mesh(&sc, xy(50, 50), col(55, 50, 50, 255));
	for (auto& segment : p.body)
	{
		if (segment == p.body[0])
			EtaCore::draw_fillrect(&sc, segment, xy(segment.x + 50, segment.y + 50), col(200, 255, 0, 255));
		else
			EtaCore::draw_fillrect(&sc, segment, xy(segment.x + 50, segment.y + 50), col(0, 255, 0, 255));

		int r = 0;
		for (auto& segment2 : p.body)
		{
			if (segment == segment2) r += 1;
		}
		if (r == 2) sc.done = true;
	}

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

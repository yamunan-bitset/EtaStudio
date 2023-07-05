#include "../../src/eta.hh"

Screen sc = {
	.title = "Snake",
	.dim = xy(900, 900),
	.bg = col(0, 0, 0, 255),
};

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
		case SDL_QUIT:
			sc.done = true;
			break;
		}
	}
}

void Eta::Loop()
{
	SDL_Rect rect(0, 0, 100, 100);
	SDL_Surface *surface = SDL_CreateRGBSurface(SDL_SWSURFACE, 8, 8, 32, 0, 0, 0, 0);
	SDL_FillRect(surface, &rect, SDL_MapRGB(surface->format, 0x00, 0x00, 0x00));
}

int main()
{
	Eta eta(sc);
	return eta.Run();
}

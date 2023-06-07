#include "eta.hh"
#include "imgui_sdl.hh"

// Screen sc = read_json("window.json");
Screen sc = {
	.title = "3-Body Simulator",
	.dim = xy(1400, 900),
	.bg = { 10, 10, 50, 255 },
	.fullscreen = false,
	.done = false
};

// 3 bodies:
Vec b1 = { 0, 0 }, b2 = { 0, 0 }, b3 = { 0, 0 };
Vecf vel1 = { 0.f, 0.f }, vel2 = { 0.f, 0.f }, vel3 = { 0.f, 0.f };
float m1 = 1.f, m2 = 1.f, m3 = 1.f;
// Loop
int mouseX, mouseY, n_wins = 0;

Box bx1 = {
  .top = xy(1100, 700),
  .bottom = xy(1300, 900)
};

// Example file for 3body simulator
void setup()
{
	// Setup
}

#include <iostream>
void loop()
{
	TTF_Font* font = TTF_OpenFont("TheSansPlain.ttf", 20);
	Msg msg = {
		.str = "Hello, World!",
		.pos = xy(100, 100),
		.font = font,
		.c = col(255, 255, 255, 255)
	}; // TODO: make a class to handle eta_run();

	// Handle:
	while (SDL_PollEvent(&sc.event))
	{
		switch (sc.event.type)
		{
			case SDL_QUIT:
				sc.done = true;
				break;
			case SDL_KEYDOWN:
				switch (sc.event.key.keysym.sym)
				{
					case SDLK_q:
						sc.done = true;
						break;
					case SDLK_h:
						message_box(&sc, "Help", "Click 3 times to spawn 3 bodies.\nThen press space to start.");
						break;
					case SDLK_SPACE:
						if (b1.x == 0 && b1.y == 0
							|| b2.x == 0 && b2.y == 0
							|| b3.x == 0 && b3.y == 0)
							message_box(&sc, "Error", "Cannot start sim.\n Are you sure you clicked 3 times?\nPress h for more details.");
						// TODO: start sim
						break;
					}
				break;
			case SDL_MOUSEBUTTONDOWN:
				SDL_GetMouseState(&mouseX, &mouseY);
				switch (sc.event.button.button)
				{
				case SDL_BUTTON_LEFT:
					if (b1.x == 0 && b1.y == 0)
					{
						b1.x = mouseX;
						b1.y = mouseY;
					}
					else if (b2.x == 0 && b2.y == 0)
					{
						b2.x = mouseX;
						b2.y = mouseY;
					}
					else if (b3.x == 0 && b3.y == 0)
					{
						b3.x = mouseX;
						b3.y = mouseY;
					}
					break;
				}
				break;
		}
	}

	if (b1.x != 0 && b1.y != 0)
	{
		draw_circ(&sc, b1, m1*10, col(200, 200, 30, 255));
	}
	if (b2.x != 0 && b2.y != 0)
	{
		draw_circ(&sc, b2, m2*10, col(30, 200, 200, 255));
	}
	if (b3.x != 0 && b3.y != 0)
	{
		draw_circ(&sc, b3, m3*10, col(200, 30, 200, 255));
	}

	ImGui::NewFrame();
	if (b1.x != 0 && b1.y != 0 && b2.x != 0 && b2.y != 0 && b3.x != 0 && b3.y != 0)
	{		
		ImGui::Begin("Planet Properties");
		{
			ImGui::Text("Properties of Yellow Planet");
			ImGui::SliderFloat("velocity x-component 1", &vel1.x, -10.f, 10.f);
			ImGui::SliderFloat("velocity y-component 1", &vel1.y, -10.f, 10.f);
			ImGui::SliderFloat("mass 1", &m1, 1.f, 10.f);
			ImGui::Spacing();
			ImGui::Text("Properties of Cyan Planet");
			ImGui::SliderFloat("velocity x-component 2", &vel2.x, -10.f, 10.f);
			ImGui::SliderFloat("velocity y-component 2", &vel2.y, -10.f, 10.f);
			ImGui::SliderFloat("mass 2", &m2, 1.f, 10.f);
			ImGui::Spacing();
			ImGui::Text("Properties of Purple Planet");
			ImGui::SliderFloat("velocity x-component 3", &vel3.x, -10.f, 10.f);
			ImGui::SliderFloat("velocity y-component 3", &vel3.y, -10.f, 10.f);
			ImGui::SliderFloat("mass 3", &m3, 1.f, 10.f);
			ImGui::End();
		}
	}
	draw_text(&sc, &msg);
	draw_box(&sc, &bx1, col(200, 100, 50, 255));
}

int main()
{
	return eta_run(&sc, setup, loop);
}
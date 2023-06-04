#include "eta.hh"
#include "imgui_sdl.hh"

// Example file for 3body simulator

static Screen sc = {
  .dim = xy(1400, 900),
  .offset = xy(20, 20),
  .bg = { 2, 2, 10, 255 },
  .fullscreen = false,
  .done = false
};

int main()
{
	// Setup
	init_sdl();
	setup_screen(&sc);

	ImGui::CreateContext();
	ImGuiSDL::Initialize(sc.impl, sc.dim.x, sc.dim.y);

	// Font
	TTF_Font* font = TTF_OpenFont("TheSansPlain.ttf", 20);
	SDL_Texture* help = gen_text(&sc, "Press h for Help", font, col(0, 255, 0, 255));
	SDL_Texture* start = gen_text(&sc, "Press space to start sim", font, col(0, 255, 0, 255));

	Msg msg[2] = {
	  {
		.str = "Hello, World!",
		.pos = xy(200, 300),
		.font = font,
		.c = col(0, 255, 0, 255)
	  },
	  {
		.str = "Bad Apple!",
		.pos = xy(400, 500),
		.font = font,
		.c = col(0, 255, 0, 255)
	  }
	};
	Box bx1 = {
	  .msgs = msg,
	  .top = xy(200, 200),
	  .bottom = xy(600, 600)
	};

	// 3 bodies:
	Vec b1 = { 0, 0 }, b2 = { 0, 0 }, b3 = { 0, 0 };
	float vel1 = 0.f, vel2 = 0.f, vel3 = 0.f;
	SDL_Texture* texture = SDL_CreateTexture(sc.impl, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_TARGET, 100, 100);
	// Loop
	int mouseX, mouseY, n_wins = 0;
	while (!sc.done)
	{
		int wheel = 0;
		ImGuiIO& io = ImGui::GetIO();

		io.DisplaySize = ImVec2(1400, 900);
		// Handle:
		while (SDL_PollEvent(&sc.event))
		{
			switch (sc.event.type)
			{
			case SDL_WINDOWEVENT:

				break;
			case SDL_MOUSEWHEEL:
				wheel = sc.event.wheel.y;
				break;
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

		int mouseX, mouseY;
		const int buttons = SDL_GetMouseState(&mouseX, &mouseY);
		io.DeltaTime = 1.0f / 60.0f;
		io.MousePos = ImVec2(static_cast<float>(mouseX), static_cast<float>(mouseY));
		io.MouseDown[0] = buttons & SDL_BUTTON(SDL_BUTTON_LEFT);
		io.MouseDown[1] = buttons & SDL_BUTTON(SDL_BUTTON_RIGHT);
		io.MouseWheel = static_cast<float>(wheel);

		// Render:
		render_sdl(&sc);

		draw_box_solid(&sc, xy(1100, 700), xy(1300, 900), col(255, 0, 0, 255));

		if (b1.x != 0 && b1.y != 0)
		{
			draw_circ(&sc, b1, 30, col(200, 200, 30, 255));
		}
		if (b2.x != 0 && b2.y != 0)
		{
			draw_circ(&sc, b2, 30, col(30, 200, 200, 255));
		}
		if (b3.x != 0 && b3.y != 0)
		{
			draw_circ(&sc, b3, 30, col(200, 30, 200, 255));
		}
		ImGui::NewFrame();
		ImGui::SetNextWindowPos(ImVec2(100, 600));		
		ImGui::Begin("Planet Properties");
		{
		  ImGui::SetWindowSize(ImVec2(500, 400));
		  ImGui::SliderFloat("B1 initial Velocity", &vel1, -10.f, 10.f);
		  ImGui::SliderFloat("B2 initial Velocity", &vel2, -10.f, 10.f);
		  ImGui::SliderFloat("B3 initial Velocity", &vel3, -10.f, 10.f);
		  ImGui::End();
		}
		// draw_box(&sc, &bx1, col(200, 100, 50, 255));
		draw_texture(&sc, help, xy(1130, 825));
		draw_texture(&sc, start, xy(1100, 850));
		draw_text(&sc, &msg[0]);
		draw_text(&sc, &msg[1]);
		
		ImGui::ShowDemoWindow();
		ImGui::Render();
		ImGuiSDL::Render(ImGui::GetDrawData());

		update_sdl(&sc);
	}
	// Quit:
	ImGuiSDL::Deinitialize();
	close_sdl(&sc);
	return 0;
}

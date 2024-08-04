# EtaStudio

## Game Engine based on SDL2 and Dear Mr ImGui
EtaStudio provides a simple and quick framework to make games and also GUI applications. It is very light weight, with the main selling point being that it requires less effort when compared to programming in barebones SDL2 and also an easy approach to integrating ImGui. 

### How to setup an EtaStudio Project
First install SDL2 and ImGui. Then copy the directory `src` into your project directory and rename it to `ETA`. In your main source file, include `"ETA/eta.hh"`. Note that EtaStudio is written in C++20. 

### Example - Basic window
```c++
#include "ETA/eta.hh"

Screen sc = { // create screen
	.title = "New Basic Window",
	.dim = xy(1000, 800),
	.bg = col(20, 0, 50, 255)
};

Box bx1 = { // create a box
	.top = xy(800, 200),
	.bottom = xy(900, 400),
	.c = col(255, 0, 0, 255),
	.fill = true
};

Msg msg1 = { // create a message box
	.str = "Hello, World!",
	.pos = xy(500, 200),
	.font = "TheSansPlain.ttf",
	.size = 20,
	.c = col(255, 255, 255, 255)
};

void Eta::Setup()
{
	// Setup code goes here
	eta_boxes.push_back(bx1); // Push the box bx1 into the stack and it will automatically be rendered. 
	eta_msgs.push_back(msg1); // Same with message boxes.
}

void Eta::Handle()
{
	while (SDL_PollEvent(&sc.event))
	{
		switch (sc.event.type)
		{
		case SDL_QUIT: sc.done = true;
		}
	}
}

void Eta::Loop()
{
	// Loop code goes here
}

void Eta::Render()
{
	// Render code goes here
	// to draw a rectangle, call the draw_fillrect function from the EtaCore library.
	EtaCore::draw_fillrect(&sc, xy(100, 200), xy(200, 300), col(255, 255, 0, 255));
}

int main()
{
	Eta eta(sc);
	return eta.Run();
}
```

### Features 
EtaStudio includes a few, yet simple features. The most phenomenal is the Box and Msg vectors. These are arrays, which would automatically render in the mainloop, without needing any effort. 
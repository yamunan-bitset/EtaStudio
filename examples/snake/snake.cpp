#include "../../src/eta.hh"

Screen sc = {
	.dim = xy(900, 900),
	.bg = col(0, 0, 0, 255),
	.title = "Snake"
};

int main()
{
	Eta eta(sc);
	return eta.Run();
}

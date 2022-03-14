#include "Game.h"



int main(int argc, char* args[])
{
	Game game;
	if (!game.Init()) return -1;

	bool end = false;
	while (!end)
	{
		end = game.Update();
		game.Draw();
		if (game.finish == true)
		{
			break;
		}
	}

	game.Release();
	return 0;
}
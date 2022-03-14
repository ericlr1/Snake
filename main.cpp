#include "Game.h"



int main(int argc, char* args[])
{
	srand(time(NULL));
	int randomObject = rand() % 5;
	int randomPos = rand() % 1020;

	Game game;
	if (!game.Init()) return -1;

	bool end = false;
	while (!end)
	{
		end = game.Update();
		game.Draw();
	}

	game.Release();
	return 0;
}
#include <list>
#include "Game.h"

Game* game = new Game();

int main()
{
	game->Load();
	game->Play();
	return 0;
}
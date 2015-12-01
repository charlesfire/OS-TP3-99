#include <vld.h>

#include "Game.h"
#include <SFML/Network.hpp>
using namespace JC9;
int main()
{
	setlocale(LC_ALL, "");
	Game game;
	game.Run();
	return EXIT_SUCCESS;
}
#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <string>
#include "game.h"

GameData game;

int main (int argc, char * args[])
{

	if (!game.Run() )
	{
		printf("failed to init game");
		exit(0);
	}

	return 0;
}
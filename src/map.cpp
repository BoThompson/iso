#include <stdio.h>
#include <string>
#include <stdlib.h>
#include <fstream>
#include <SDL.h>
#include <SDL_image.h>
#include "texture.h"
#include "tiles.h"
#include "utils.h"
#include "game.h"
#include "map.h"

extern GameData game;
extern enum Effects;

const int TILE_WIDTH = 64; //error lnk2001
const int TILE_HEIGHT = 31;

const int TOTAL_TILE_SPRITES = 12;

/// <summary>
/// Initializes a new instance of the <see cref="MapData"/> class.
/// </summary>
MapData::MapData()
{
	m_tilesheet = TilesheetData();
}

/// <summary>
/// Setups the specified file.
/// </summary>
/// <param name="file">The filename containing the map data.</param>
/// <returns>returns true if if successfully done, false if not</returns>
bool MapData::Setup(std::string file)
{
	bool success = true;
	int x = 0;
	int y = 0;
	int tx = 0;
	int ty = 0;

	std::ifstream mapFile (file.c_str());

	if (mapFile == NULL)
	{
		printf("unable to load map file\n");
		success = false;
	}

	m_tilesheet.LoadTileSheet("image/tile.png", game.GameRender());

	for (y = 0; y < 16; y++)
	{
		for (x = 0; x < 12 ; x++)
		{
			int tileType = -1;

			mapFile >> tileType;

			if (mapFile.fail())
			{
			   printf( "Error loading map: Unexpected end of file!\n" );
			   success = false;
			   break;
			}

			/*if ((tileType >= 0) && (tileType < TOTAL_TILE_SPRITES))
			{
				tilesheet.tileset[i] = new TileData(x, y, tileType);
			}*/
			if ((tileType >= 0) && (tileType < TOTAL_TILE_SPRITES))
			{
				m_tileSet[x][y] = new TileData(tileType);
			}
			else
			{
				printf( "Error loading map: Invalid tile type found" );
				success = false;
				break;
			}
		}

		/*
		x += TILE_WIDTH;

		if (x >= LEVEL_WIDTH)
		{
			x = 0;
			y += TILE_HEIGHT;
		}*/
	}
	
	mapFile.close();
	return success;
}

/// <summary>
/// Renders the specified renderer.
/// </summary>
/// <param name="renderer">The specified renderer.</param>
/// <param name="x">The x coordinate.</param>
/// <param name="y">The y coordinate.</param>
void MapData::Render(SDL_Renderer * renderer, int x, int y)
{
	SDL_Rect r;
	r.w = TILE_WIDTH;
	r.h = TILE_HEIGHT;
	r.x = x;
	r.y = y;
	int rx;
	for (int y = 0; y < 15; y++) //test
	{
		//new
		//r.y += 16;

		int ty = y;
		rx = r.x;
		for (int tx = 0; ty >= 0; tx++)
		{
			if (tx > 11) // glitch fix, tries to go in 12th part of tileset when there is none, access mem. violation. // y+= 16, x-= 31
				break;

			//new
			//r.x -= 31;

			//r.x -= 31 + (tx*TILE_WIDTH);
			//r.y += 16 + (ty*TILE_HEIGHT);

			m_tilesheet.Render(renderer, m_tileSet[tx][ty]->tilenum, r);
			r.x += 64;

			ty--;
		}
		r.x = rx - 32;
		r.y += 16;

		
	}
}

bool MapData::Close()
{
	// will close the map
	return true;
}
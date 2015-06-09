#include <stdio.h>
#include <string>
#include <stdlib.h>
#include <fstream>
#include <SDL.h>
#include <SDL_image.h>
#include "texture.h"
#include "utils.h"
#include "game.h"
#include "tiles.h"

//using namespace std;
extern enum Effects;

const int TILE_WIDTH = 64; //error lnk2001
const int TILE_HEIGHT = 31;

/// <summary>
/// Initializes a new instance of the <see cref="TileData"/> struct.
/// </summary>
/// <param name="tileType">Type of the tile.</param>
TileData::TileData( int tileType)
{
	m_tileType = tileType; // will happen in tile drawing function
	tilenum = -1; 
}

/// <summary>
/// Initializes a new instance of the <see cref="TilesheetData"/> class.
/// </summary>
TilesheetData::TilesheetData()
{
	m_textureTile = TextureData();
}

/// <summary>
/// Renders the tilesheet to the specified renderer.
/// </summary>
/// <param name="renderer">The renderer.</param>
/// <param name="num">The tile number.</param>
/// <param name="dest">Where it start appearing.</param>
void TilesheetData::Render(SDL_Renderer * renderer, int num, SDL_Rect dest)
{
	SDL_Rect source;

	if(num == -1)
		return;

	source.x = (num * TILE_WIDTH) % m_textureTile.Width();
	source.y = (num * TILE_WIDTH) / m_textureTile.Width() * TILE_HEIGHT;
	source.w = TILE_WIDTH;
	source.h = TILE_HEIGHT;

	m_textureTile.Render(renderer, &source, &dest);
}

/// <summary>
/// Loads the tilesheet from filename, for the specified renderer.
/// </summary>
/// <param name="path">The filename</param>
/// <param name="renderer">The specified renderer</param>
void TilesheetData::LoadTileSheet(std::string path, SDL_Renderer * renderer)
{
	//SDL_Texture * texture = LoadTexture(path, renderer);
	m_textureTile = TextureData();
	m_textureTile.Setup(LoadTexture(path, renderer));
	//TextureData nTexture;

	//if(nTexture.Setup(LoadTexture(path, renderer)))
		//return &nTexture;
	
}
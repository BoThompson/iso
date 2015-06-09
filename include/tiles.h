#ifndef _TILES_H_
#define _TILES_H_

/// <summary>
/// Individual Tile
/// </summary>
struct TileData //tiles.h/cpp
{
	//SDL_Rect m_cBox;
	//int elevation;
	/// <summary>
	/// The type of the tile
	/// </summary>
	int m_tileType;
	/// <summary>
	/// The tile #: will determine where on the spritesheet to clip
	/// </summary>
	int tilenum; // similar to frame #

public:
	TileData(int tileType); // init a tile
};

/// <summary>
/// Sheet of Tiles
/// </summary>
class TilesheetData //tiles.h/cpp
{
	// const char * m_filename;
	// int inuse;
	int m_framesperline; // for going through the tilesheet and finding the frame/tile
	/// <summary>
	/// The tilesheet texture
	/// </summary>
	TextureData m_textureTile; // the tilesheet itself
public:
	TilesheetData();
	void LoadTileSheet(std::string path, SDL_Renderer * renderer); // load the tilesheet file
	void Render(SDL_Renderer * renderer, int num, SDL_Rect dest); // render a certain frame
};

#endif
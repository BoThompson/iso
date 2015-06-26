#ifndef _TILES_H_
#define _TILES_H_

#define TILE_WIDTH 64
#define TILE_HEIGHT 31
/// <summary>
/// Individual Tile
/// </summary>
class TileData //tiles.h/cpp
{
	/// <summary>
	/// The height of the tile
	/// </summary>
	int m_elevation;
	/// <summary>
	/// The type of the tile
	/// </summary>
	int m_tileType;
	/// <summary>
	/// Designated list of things that is on this tile
	/// </summary>
	EntityData * current; // current entity being pointed at
	EntityData * m_onTile; // beginning of list, there may be more than one item on top of this tile
	EntityData * temp; // temp pointer for functional purposes

	/* The following are values for each tile's center and boundaries */
	int centerX; // defined by equation
	int centerY;

	int leftBound; //centerX - 32
	int rightBound; // centerX + 32
	int topBound; // centerY - 16?
	int bottomBound; // centerY + 16?

public:
	TileData(int tileType); // function to init. a tile
	
	void SetHeight(short elevation);
	void SetType (int tileType);
	
	int GetHeight ();
	int GetType ();

	void AddToList(EntityData * ent);
	EntityData *RemoveFromList(EntityData * ent);
	void PrintLinkList();
	EntityData *OnTile();
	void SetBoundaries(int centerX, int centerY);
	
	int CenterX();
	int CenterY();
	int RightBound();
	int TopBound();
	int LeftBound();
	int BottomBound();
};

/// <summary>
/// Sheet of Tiles
/// </summary>
class TilesheetData //tiles.h/cpp
{
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
#ifndef _MAP_H_
#define _MAP_H_

#define MAX_MAP_WIDTH 20
#define MAX_MAP_HEIGHT 20

/// <summary>
/// Map contaning data about sheet of files
/// </summary>
class MapData //map.h/cpp
{
	int m_height;
	int m_width;

	char m_filename[80]; // file holding the design/coordinates
	/// <summary>
	/// The tile set to determine how the tiles are shown and what tiles are shown.
	/// </summary>
	TileData * m_tileSet[MAX_MAP_WIDTH][MAX_MAP_HEIGHT]; // where the data from the file will be saved to
	/// <summary>
	/// Set of heights for each tile
	/// </summary>
	short m_heightSet[MAX_MAP_WIDTH][MAX_MAP_HEIGHT];
	/// <summary>
	/// Reference to tilesheet image file
	/// </summary>
	TilesheetData m_tilesheet; // reference to tilesheet image file
public:
	int m_x;
	int m_y;

	MapData();
	bool Setup(std::string file);
	void Render(SDL_Renderer * renderer, int x, int y);
	bool Close();
	bool Save();

	// to be removed or renamed
	void SetHeight(int tx, int ty, short elevation);
	void SetType(int tx, int ty, int tileType);
	
	int GetHeight(int tx, int ty);
	int GetType (int tx, int ty);
	
	bool AddEntityToTile(int tx, int ty, EntityData * ent);
	bool RemoveEntityFromTile(int tx, int ty, EntityData * ent);
	void PrintOnTile(int tx, int ty);
};

#endif
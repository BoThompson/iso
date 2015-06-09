#ifndef _MAP_H_
#define _MAP_H_

/// <summary>
/// Map contaning data about sheet of files
/// </summary>
class MapData //map.h/cpp
{
	// int maxTiles;
	const char * m_filename; // file holding the design/coordinates
	/// <summary>
	/// The tile set to determine how the tiles are shown and what tiles are shown.
	/// </summary>
	TileData * m_tileSet[12][16]; // 192 tiles, where the data from the file will be saved to
	/// <summary>
	/// Reference to tilesheet image file
	/// </summary>
	TilesheetData m_tilesheet; // reference to tilesheet image file
public:
	MapData();
	bool Setup(std::string file);
	void Render(SDL_Renderer * renderer, int x, int y);
	bool Close();
};

#endif //_MAP_H_
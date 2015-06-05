#ifndef _GAME_H_
#define _GAME_H_

#include <SDL_ttf.h>

/// <summary>
/// List of possible Texture Effects
/// </summary>
enum Effects
{
	TEXFX_FADEIN,
	TEXFX_FADEOUT,
	MAX_TEXFX
};

/*
class TileData
{
	SDL_Rect m_box;
	int m_type;

public:
	TileData( int x, int y, int tileType );
	void Render (SDL_Renderer * renderer);
	int GetType();
	SDL_Rect GetBox();
};

class MapData //map information
{
	int m_width;
	int m_height;
	const int m_totalTiles;
	const int m_totalTileSprites;

	TileData * m_tileset[192];

public:
	bool Setup(std::string path);
	void Render(SDL_Renderer * renderer, SDL_Rect camera, SDL_Rect viewport);
	void DrawTile(int tx, int ty); 
	bool Close();

};
*/

/// <summary>
/// GameData class
/// </summary>
class GameData
{	
	/// <summary>
	/// Renderer for the game
	/// </summary>
	SDL_Renderer * m_renderer;	
	/// <summary>
	/// Window for the game
	/// </summary>
	SDL_Window * m_window;	
	/// <summary>
	/// Splash screen texture
	/// </summary>
	SDL_Texture * m_texture;	
	/// <summary>
	/// SDL polled events
	/// </summary>
	SDL_Event m_events;	
	/// <summary>
	/// If the game is done
	/// </summary>

	TTF_Font * m_font;

	bool	m_done;

	void Poll();
	void Update();
	void Draw();
	bool Setup();
	void Shutdown();

public: 
	bool Run();
	SDL_Renderer * GameRender();
	SDL_Texture * LoadImage(std::string path);
};


/// <summary>
/// TextureData Class
/// </summary>
class TextureData
{
	SDL_Texture * m_texture;
	int m_width;
	int m_height;
	int m_flags;
	long m_texfxStart[MAX_TEXFX];
	long m_texfxEnd[MAX_TEXFX];

public:
	TextureData();
	//~TextureData();

	bool Setup (SDL_Texture * texture);
	void Free();
	void SetColor(Uint8 red, Uint8 green, Uint8 blue);
	void SetBlendMode(SDL_BlendMode blending);
	void SetAlpha(Uint8 alpha);
	void Render (SDL_Renderer * renderer, SDL_Rect * src, SDL_Rect * dest);
	void NRender (int x, int y, SDL_Renderer * renderer, SDL_Rect * clip = NULL, double angle = 0.0, SDL_Point * center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE );
	void SetEffect(int flag, long start, long final);
	void UpdateEffects();
	int Width();
	int Height();
	int SetWidth(int width);
	int SetHeight(int height);
};

/// <summary>
/// Individual Tile
/// </summary>
struct TileData
{
	SDL_Rect m_cBox;
	//int elevation;
	int m_tileType; // type of tile
	int tilenum; // similar to frame #

public:
	TileData(int tileType); // init a tile
};

/// <summary>
/// Sheet of Tiles
/// </summary>
class TilesheetData
{
	// const char * m_filename;
	// int inuse;
	int m_framesperline; // for going through the tilesheet and finding the frame/tile
	TextureData m_textureTile; // the tilesheet itself
public:
	TilesheetData();
	void LoadTileSheet(std::string path, SDL_Renderer * renderer); // load the tilesheet file
	void Render(SDL_Renderer * renderer, int num, SDL_Rect dest); // render a certain frame
};

/// <summary>
/// Map contaning data about sheet of files
/// </summary>
class MapData
{
	// int maxTiles;
	const char * m_filename; // file holding the design/coordinates
	TileData * m_tileSet[12][16]; // 192 tiles, where the data from the file will be saved to
	TilesheetData m_tilesheet; // reference to tilesheet image file
public:
	MapData();
	bool Setup(std::string file);
	void Render(SDL_Renderer * renderer, int x, int y);
	bool Close();
};

#endif _GAME_H_
#ifndef _GAME_H_
#define _GAME_H_

#include <SDL_ttf.h> // Error 2	error C4430: missing type specifier - int assumed. Note: C++ does not support default-int

/// <summary>
/// List of possible Texture Effects
/// </summary>
enum Effects
{
	TEXFX_FADEIN,
	TEXFX_FADEOUT,
	MAX_TEXFX
};

/// <summary>
/// GameData class
/// </summary>
class GameData //game.h/cpp
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
	/// The game's camera
	/// </summary>
	SDL_Rect m_camera;
	/// <summary>
	/// SDL polled events
	/// </summary>
	SDL_Event m_events;	
	/// <summary>
	/// The game's current map
	/// </summary>
	struct MapData * m_currentMap; // cirucular dependency, using struct instead
	/// <summary>
	/// If the game is done
	/// </summary>
	bool	m_done;
	/// <summary>
	/// The m_font
	/// </summary>
	TTF_Font * m_font;

	void Poll();
	void Update();
	void Draw();
	bool Setup();
	void Shutdown();

public: 
	bool Run();
	
	SDL_Renderer * GameRender();
	SDL_Texture * LoadImage(std::string path);
	SDL_Rect Camera();
	MapData * CurrentMap();
	
	void SetCamera(int x, int y);
	void End();
};

#endif //_GAME_H_
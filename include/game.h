#ifndef _GAME_H_
#define _GAME_H_

class GameData
{
	SDL_Renderer * m_renderer;
	SDL_Window * m_window;
	SDL_Texture * m_texture;
	SDL_Event m_events;
	bool	m_done;

	void Poll();
	void Update();
	void Draw();
	bool Setup();
	void Shutdown();

public: 
	bool Run();

};

#endif _GAME_H_
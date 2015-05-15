#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <string>
#include "game.h"
#include "utils.h"


/// <summary>
/// Setups this instance.
/// </summary>
/// <returns> returns false if an error occurs</returns>
bool GameData::Setup()
{
	bool success = true;
	int imgFlags = IMG_INIT_PNG;

	if (SDL_Init (SDL_INIT_VIDEO) < 0 )
	{
		printf("SDL failed: ", SDL_GetError() );
		success = false;
	}
	else
	{
		if ( !SDL_SetHint (SDL_HINT_RENDER_SCALE_QUALITY, "1" ))
			printf(" linear texture filtering fail ");
		
		m_window = SDL_CreateWindow ("SDL Render", 
			SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640,
			480, SDL_WINDOW_SHOWN);
		
		if (m_window == NULL)
		{
			printf("window failed: %s\n", SDL_GetError() );
			success = false;
		}
		else
		{
			m_renderer = SDL_CreateRenderer (m_window, -1, SDL_RENDERER_ACCELERATED);
			if (m_renderer == NULL)
			{
				printf("renderer fail: %s\n", SDL_GetError());
				success = false;
			}
			else
			{
				SDL_SetRenderDrawColor(m_renderer, 0xFF, 0xFF, 0xFF, 0xFF);
				
				// why the comparison?
				if ( !(IMG_Init(imgFlags) & imgFlags) )
				{
					printf("sdl image fail: %s\n", SDL_GetError());
					success = false;
				}
				else
				{
					m_texture = LoadTexture("texture.png", m_renderer);
					if (m_texture == NULL)
					{
						printf("load texture fail\n");
						success = false;
					}
				}
			}
		}
	}
	return success;
}

/// <summary>
/// Polls the user for input.
/// </summary>
void GameData::Poll()
{
	while (SDL_PollEvent (&m_events) != 0)
	{
		if (m_events.type == SDL_QUIT)
			m_done = true;
	}
}

void GameData::Update ()
{

}

/// <summary>
/// Draws to the game.
/// </summary>
void GameData::Draw()
{
	SDL_RenderClear(m_renderer);

	SDL_RenderCopy(m_renderer, m_texture, NULL, NULL);

	SDL_RenderPresent(m_renderer);
}

/// <summary>
/// Shuts down game.
/// </summary>
void GameData::Shutdown()
{
	SDL_DestroyTexture( m_texture );
	m_texture = NULL;

	SDL_DestroyRenderer( m_renderer );
	SDL_DestroyWindow ( m_window );

	m_window = NULL;
	m_renderer = NULL;

	IMG_Quit();
	SDL_Quit();
}

/// <summary>
/// Runs the game.
/// </summary>
/// <returns></returns>
bool GameData::Run()
{
	if (!Setup() )
	{
		return false;
	}

	do
	{
		Poll();
		Update();
		Draw();

	}while(!m_done);
}
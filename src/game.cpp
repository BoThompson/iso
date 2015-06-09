#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <stdio.h>
#include <string>
#include <fstream>
#include "texture.h"
#include "tiles.h"
#include "utils.h"
#include "map.h"
#include "game.h"

using namespace std;

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

const int LEVEL_WIDTH = 1280;
const int LEVEL_HEIGHT = 960;

const int TILE_WIDTH = 64;
const int TILE_HEIGHT = 31;
const int TOTAL_TILES = 192;
const int TOTAL_TILE_SPRITES = 12;

const int TILE_RED = 0;
const int TILE_GREEN = 1;
const int TILE_BLUE = 2;
const int TILE_CENTER = 3;
const int TILE_TOP = 4;
const int TILE_TOPRIGHT = 5;
const int TILE_RIGHT = 6;
const int TILE_BOTTOMRIGHT = 7;
const int TILE_BOTTOM = 8;
const int TILE_BOTTOMLEFT = 9;
const int TILE_LEFT = 10;
const int TILE_TOPLEFT = 11;

TextureData texture1;
TextureData texture2;
TextureData textureText;
TextureData textureArrow;
TextureData textureColorkey;
TextureData textureTiles;
TextureData textureColors;

MapData tileMap1;

extern GameData game;
//TileData* tileSet[ TOTAL_TILES ];
//SDL_Rect tileClips[ TOTAL_TILE_SPRITES ];

double degrees = 0;
SDL_RendererFlip flipType = SDL_FLIP_NONE;

//int tileLocation = 0;

//Modulation components
Uint8 r = 255;
Uint8 g = 255;
Uint8 b = 255;

/*
Uint8 a = 0;
int final = 5000;
int start = 0;
*/

int current;
bool nextscreen;

/// <summary>
/// Initializes a new instance of the <see cref="TextureData"/> class.
/// </summary>
TextureData::TextureData()
{
	m_texture = NULL;
	m_width = 0;
	m_height = 0;
}

/// <summary>
/// Sets the width.
/// </summary>
/// <param name="width">The width.</param>
/// <returns>returns the new width</returns>
int TextureData::SetWidth(int width)
{
	m_width = width;
	return m_width;
}

/// <summary>
/// Sets the height.
/// </summary>
/// <param name="height">The height.</param>
/// <returns>returns the new height</returns>
int TextureData::SetHeight(int height)
{
	m_height = height;
	return height;
}

/// <summary>
/// Sets initial color using RGB coordinates.
/// </summary>
/// <param name="red">The red coordinate.</param>
/// <param name="green">The green coordinate.</param>
/// <param name="blue">The blue coordinate.</param>
void TextureData::SetColor(Uint8 red, Uint8 green, Uint8 blue)
{
	SDL_SetTextureColorMod(m_texture, red, green, blue);
}

/// <summary>
/// Sets the blending mode mainly for alpha blending.
/// </summary>
/// <param name="blending">The blending.</param>
void TextureData::SetBlendMode(SDL_BlendMode blending)
{
	SDL_SetTextureBlendMode(m_texture, blending);
}

//(int x, int y, SDL_Rect* clip, double angle, SDL_Point * center, SDL_RendererFlip flip);
/// <summary>
/// Renders the texture to the specified render.
/// </summary>
/// <param name="renderer">The renderer specified.</param>
/// <param name="src">The source rectangle or what has been clipped. ex. frame of spritesheet.</param>
/// <param name="dest">The dest rectangle or where it will show up on screen.</param>
void TextureData::Render (SDL_Renderer * renderer, SDL_Rect *src, SDL_Rect *dest)
{
	SDL_RenderCopy(renderer, m_texture, src, dest);
}

/// <summary>
/// Renders the texture with the ability of rotationa and flipping.
/// </summary>
/// <param name="x">Where on the x coordinate it should appear.</param>
/// <param name="y">Where on the y coordinate it should appear.</param>
/// <param name="renderer">The renderer specified.</param>
/// <param name="clip">The clip: cropping the texture.</param>
/// <param name="angle">The angle: for rotation.</param>
/// <param name="center">The center of rotation.</param>
/// <param name="flip">How the texture should flip.</param>
void TextureData::NRender (int x, int y, SDL_Renderer * renderer, SDL_Rect * clip, double angle, SDL_Point * center, SDL_RendererFlip flip )
{
	SDL_Rect renderQuad = {x, y, m_width, m_height};

	if (clip != NULL)
	{
		renderQuad.w = clip->w;
		renderQuad.h = clip->h;
	}

	SDL_RenderCopyEx(renderer, m_texture, clip, &renderQuad, angle, center, flip);
}

/// <summary>
/// Sets the alpha.
/// </summary>
/// <param name="alpha">The alpha rate requested.</param>
void TextureData::SetAlpha(Uint8 alpha)
{
	SDL_SetTextureAlphaMod(m_texture, alpha);
}

/// <summary>
/// Width of the texture.
/// </summary>
/// <returns>returns the width of the texture</returns>
int TextureData::Width()
{
	return m_width;
}

/// <summary>
/// Height of the texture.
/// </summary>
/// <returns>returns the height of the texture</returns>
int TextureData::Height()
{
	return m_height;
}

/// <summary>
/// Tell the game to start the effect.
/// </summary>
/// <param name="flag">The flag: which effect to do.</param>
/// <param name="start">The start: what time to start.</param>
/// <param name="final">The final: what time to end.</param>
void TextureData::SetEffect(int flag, long start, long final)
{
	if(flag >= MAX_TEXFX)
	{
		//error!!!!
		printf("requested effect out of bounds");
		return;
	}

	SET_BFLAG(m_flags, flag);
	m_texfxStart[flag] = start;
	m_texfxEnd[flag] = final;
}
/// <summary>
/// Does whatever effect was called in SetEffect.
/// </summary>
void TextureData::UpdateEffects()
{
	Uint8 a;
	long currentTime = SDL_GetTicks();
	for (int flag = 0; flag < MAX_TEXFX; flag++)
	{
		if (!IS_BSET(m_flags, flag))
			continue;
		if (m_texfxEnd[flag] < currentTime)
		{
			REMOVE_BFLAG(m_flags, flag);
			continue;
		}

		if (m_texfxStart[flag] > currentTime)
			continue;

		switch (flag)
		{
			case TEXFX_FADEIN:
			{
				a = 255 * (currentTime - m_texfxStart[flag])/(m_texfxEnd[flag]- m_texfxStart[flag]);
				SetAlpha( a );
				break;
			}
			case TEXFX_FADEOUT:
			{
				a = 255 - 255 * (SDL_GetTicks() - m_texfxStart[flag])/(m_texfxEnd[flag]- m_texfxStart[flag]);
				SetAlpha( a );
				if (a < 15)
				{
					nextscreen = true;
				}
				break;
			}
		}
	}
}

/// <summary>
/// Sets up the game. Opens libraries needed to run, and files to initially open.
/// </summary>
/// <returns>
/// returns false if an error occurs
/// </returns>
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
				
				if ( !(IMG_Init(imgFlags) & imgFlags) )
				{
					printf("sdl image fail: %s\n", SDL_GetError());
					success = false;
				}

				if ( TTF_Init() == -1 )
				{
					printf("SDL_ttf failed: %s\n", IMG_GetError() );
					success = false;
				}
				else
				{
					if (!texture1.Setup(LoadTexture("image/landscape.png", m_renderer)))
					{
						printf("load texture fail\n");
						success = false;
					}
					else 
					{
						texture1.SetBlendMode(SDL_BLENDMODE_BLEND);
						texture1.SetEffect(TEXFX_FADEIN, 0, 6000);
						//texture1.SetEffect(TEXFX_FADEOUT, 12000, 16000);
					}

					if (!texture2.Setup(LoadTexture("fadein.png", m_renderer)))
					{
						printf("load texture fail\n");
						success = false;
					}

					m_font = NULL;
					m_font = TTF_OpenFont("ttf/comme.thin.ttf", 20);
					if (m_font == NULL)
					{
						printf("failed to load font: %s\n", TTF_GetError() );
						success = false;
					}
					else
					{
						SDL_Color fontColor = {0,0,0};
						if (!textureText.Setup(LoadTexture("Click anywhere to continue.", m_font, fontColor, m_renderer)))
						{
							printf("failed to render text\n");
							success = false;
						}

						if (!tileMap1.Setup("lazy.map"))
						{
							printf("failed to load map file :: %s\n", SDL_GetError());
							success = false;
						}

						
						if (!textureTiles.Setup(LoadTexture("tiles.png", m_renderer)))
						{
							success = false;
						}

						if (!textureArrow.Setup(LoadTexture("arrow.png", m_renderer)))
						{
							success = false;
						}

						if (!textureColorkey.Setup(LoadTexture("foo.png", m_renderer)))
						{
							success = false;
						}
						if (!textureColors.Setup(LoadTexture("colors.png", m_renderer)))
						{
							success = false;
						}
					}
				}
			}
		}
	}
	return success;
}//Unhandled exception at 0x000000f0 in iso_project.exe: 0xC0000005: Access violation fixed.

/// <summary>
/// Event polling: clicking the close button, clicking the mouse or pressing a key is handled here.
/// </summary>
void GameData::Poll()
{
	while (SDL_PollEvent (&m_events) != 0)
	{
		if (m_events.type == SDL_QUIT)
			m_done = true;
		else
		{
			switch (m_events.type)
			{

			case SDL_MOUSEBUTTONDOWN:
				{
					texture1.SetEffect(TEXFX_FADEOUT, 10 + (int)SDL_GetTicks(), 1000 + (int)SDL_GetTicks());
					
					textureText.Free();
				}

			case SDL_KEYDOWN:
				{
					switch (m_events.key.keysym.sym)
					{
					case SDLK_a:
						degrees -= 60;
						break;
					case SDLK_d:
						degrees += 60;
						break;
					case SDLK_q:
						flipType = SDL_FLIP_HORIZONTAL;
						break;
					case SDLK_w:
						flipType = SDL_FLIP_NONE;
						break;
					case SDLK_e:
						flipType = SDL_FLIP_VERTICAL;
						break;

					case SDLK_r:
						r += 32;
						break;
					case SDLK_g:
						g += 32;
						break;
					case SDLK_b:
						b += 32;

					case SDLK_t:
						r -= 32;
						break;
					case SDLK_h:
						g -= 32;
						break;
					case SDLK_n:
						b -=32;
						break;
					}
				}
			}
		}
	}
}

/// <summary>
/// Updates game, time, etc.
/// </summary>
void GameData::Update ()
{
	current = SDL_GetTicks();
	
	texture1.UpdateEffects();
}

// 255 - 255 * (current - start)/(final - start)
// 255 * (current - start)/(final - start)
// all alpha - alpha that should've been lost

/// <summary>
/// Games the render.
/// </summary>
/// <returns>
/// returns the main renderer
/// </returns>
SDL_Renderer * GameData::GameRender()
{
	return m_renderer;
}

/// <summary>
/// Loads the image.
/// </summary>
/// <param name="path">The filename of the image.</param>
/// <returns>returns the SDL Texture resulting from the load Texture func.</returns>
SDL_Texture * GameData::LoadImage(std::string path)
{
	return LoadTexture(path, m_renderer);
}
/// <summary>
/// Draws to the game every frame.
/// </summary>
void GameData::Draw()
{
	SDL_Rect r;
	SDL_SetRenderDrawColor (m_renderer, 0xFF, 0xFF, 0xFF, 0xFF);
	SDL_RenderClear(m_renderer);
	
	texture1.Render (m_renderer, NULL, NULL);
	r.x = 320;
	r.y = 400;
	r.w = 0;
	r.h = 0;
	textureText.Render(m_renderer, &r, NULL);

	tileMap1.Render(m_renderer, 320, 240);
	
	// color modulation
	/*textureColors.SetColor(r, g, b);
	textureColors.Render(20, 20, m_renderer);*/
	
	// show tiles
	/*for (int i = 0; i < TOTAL_TILES; ++i)
	{
		tileSet[i]->Render(m_renderer);
	}*/

	//map1.Render();

	/*for (int i = 0; i < TOTAL_TILES; ++i)
	{
		for (tileLocation = 0; tileLocation < 18; tileLocation++)
		{
			int ty = tileLocation;
			for (int tx = 0; ty >= 0; tx++)
			{
				tileSet[i]->Render(m_renderer);
			}
		}
	}*/

	// rotation and flipping
	/*textureArrow.NRender(100, 100, m_renderer, NULL, degrees, NULL, flipType);*/

	// color key
	/*textureColorkey.Render(100, 200, m_renderer);*/

	if (nextscreen)
	{
		texture2.Render(m_renderer, NULL, NULL);
	}
	
	//SDL_RenderCopy(m_renderer, m_texture, NULL, NULL);
	
	// viewports
	/*
		//Top left corner viewport
		SDL_Rect topLeftViewport;
		topLeftViewport.x = 0;
		topLeftViewport.y = 0;
		topLeftViewport.w = SCREEN_WIDTH / 2;
		topLeftViewport.h = SCREEN_HEIGHT / 2;
		SDL_RenderSetViewport( m_renderer, &topLeftViewport );
				
		//Render texture to screen
		texture1.Render (0, 0, m_renderer);


		//Top right viewport
		SDL_Rect topRightViewport;
		topRightViewport.x = SCREEN_WIDTH / 2;
		topRightViewport.y = 0;
		topRightViewport.w = SCREEN_WIDTH / 2;
		topRightViewport.h = SCREEN_HEIGHT / 2;
		SDL_RenderSetViewport( m_renderer, &topRightViewport );
				
		//Render texture to screen
		texture1.Render (0, 0, m_renderer);


		//Bottom viewport
		SDL_Rect bottomViewport;
		bottomViewport.x = 0;
		bottomViewport.y = SCREEN_HEIGHT / 2;
		bottomViewport.w = SCREEN_WIDTH;
		bottomViewport.h = SCREEN_HEIGHT / 2;
		SDL_RenderSetViewport( m_renderer, &bottomViewport );

				
		//Render texture to screen
		texture1.Render (0, 0, m_renderer);
	*/

	SDL_RenderPresent(m_renderer);
}

/// <summary>
/// Shuts down game and its depending libraries, then closes window.
/// </summary>
void GameData::Shutdown()
{
	SDL_DestroyTexture( m_texture );
	m_texture = NULL;

	//textureText.Free();

	TTF_CloseFont(m_font);
	m_font = NULL;

	SDL_DestroyRenderer( m_renderer );
	SDL_DestroyWindow ( m_window );

	//need to deallocate TextureData

	m_window = NULL;
	m_renderer = NULL;

	TTF_Quit();
	IMG_Quit();
	SDL_Quit();

	exit(0);
}

/// <summary>
/// Runs the game.
/// </summary>
/// <returns>
/// at this point nothing, may be changed to void function
/// </returns>
bool GameData::Run()
{
	if (!Setup() )
	{
		printf("setup\n");
		return false;
	}

	do
	{
		Poll();
		Update();
		Draw();

	}while(!m_done);

	Shutdown();
}

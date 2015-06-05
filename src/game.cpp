#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <stdio.h>
#include <string>
#include <fstream>
#include "game.h"
#include "utils.h"

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

TextureData::TextureData()
{
	m_texture = NULL;
	m_width = 0;
	m_height = 0;
}

int TextureData::SetWidth(int width)
{
	m_width = width;
	return m_width;
}

int TextureData::SetHeight(int height)
{
	m_height = height;
	return height;
}

void TextureData::SetColor(Uint8 red, Uint8 green, Uint8 blue)
{
	SDL_SetTextureColorMod(m_texture, red, green, blue);
}

void TextureData::SetBlendMode(SDL_BlendMode blending)
{
	SDL_SetTextureBlendMode(m_texture, blending);
}

//(int x, int y, SDL_Rect* clip, double angle, SDL_Point * center, SDL_RendererFlip flip);
void TextureData::Render (SDL_Renderer * renderer, SDL_Rect *src, SDL_Rect *dest)
{
	SDL_RenderCopy(renderer, m_texture, src, dest);
	// SDL_RenderCopyEx();
}

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

void TextureData::SetAlpha(Uint8 alpha)
{
	SDL_SetTextureAlphaMod(m_texture, alpha);
}

int TextureData::Width()
{
	return m_width;
}

int TextureData::Height()
{
	return m_height;
}

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
TilesheetData::TilesheetData()
{
	m_textureTile = TextureData();
}

void TilesheetData::LoadTileSheet(std::string path, SDL_Renderer * renderer)
{
	//SDL_Texture * texture = LoadTexture(path, renderer);
	m_textureTile = TextureData();
	m_textureTile.Setup(LoadTexture(path, renderer));
	//TextureData nTexture;

	//if(nTexture.Setup(LoadTexture(path, renderer)))
		//return &nTexture;
	
}

TileData::TileData( int tileType)
{
	m_tileType = tileType; // will happen in tile drawing function
	tilenum = -1; 
}

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

MapData::MapData()
{
	m_tilesheet = TilesheetData();
}

bool MapData::Setup(std::string file)
{
	bool success = true;
	int x = 0;
	int y = 0;
	int tx = 0;
	int ty = 0;

	std::ifstream mapFile (file.c_str());

	if (mapFile == NULL)
	{
		printf("unable to load map file\n");
		success = false;
	}

	m_tilesheet.LoadTileSheet("image/tile.png", game.GameRender());

	for (y = 0; y < 16; y++)
	{
		for (x = 0; x < 12 ; x++)
		{
			int tileType = -1;

			mapFile >> tileType;

			if (mapFile.fail())
			{
			   printf( "Error loading map: Unexpected end of file!\n" );
			   success = false;
			   break;
			}

			/*if ((tileType >= 0) && (tileType < TOTAL_TILE_SPRITES))
			{
				tilesheet.tileset[i] = new TileData(x, y, tileType);
			}*/
			if ((tileType >= 0) && (tileType < TOTAL_TILE_SPRITES))
			{
				m_tileSet[x][y] = new TileData(tileType);
			}
			else
			{
				printf( "Error loading map: Invalid tile type found" );
				success = false;
				break;
			}
		}

		/*
		x += TILE_WIDTH;

		if (x >= LEVEL_WIDTH)
		{
			x = 0;
			y += TILE_HEIGHT;
		}*/
	}
	
	mapFile.close();
	return success;
}

void MapData::Render(SDL_Renderer * renderer, int x, int y)
{
	SDL_Rect r;
	r.w = TILE_WIDTH;
	r.h = TILE_HEIGHT;
	r.x = x;
	r.y = y;
	int rx;
	for (int y = 0; y < 15; y++) //test
	{
		//new
		//r.y += 16;

		int ty = y;
		rx = r.x;
		for (int tx = 0; ty >= 0; tx++)
		{
			if (tx > 11) // glitch fix, tries to go in 12th part of tileset when there is none, access mem. violation. // y+= 16, x-= 31
				break;

			//new
			//r.x -= 31;

			//r.x -= 31 + (tx*TILE_WIDTH);
			//r.y += 16 + (ty*TILE_HEIGHT);

			m_tilesheet.Render(renderer, m_tileSet[tx][ty]->tilenum, r);
			r.x += 64;

			ty--;
		}
		r.x = rx - 32;
		r.y += 16;

		
	}
}

/*
TileData::TileData( int tileType)
{

	m_tileType = tileType;
}*/

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
}//Unhandled exception at 0x000000f0 in iso_project.exe: 0xC0000005: Access violation.

/// <summary>
/// Polls the user for input.
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

void GameData::Update ()
{
	current = SDL_GetTicks();
	
	texture1.UpdateEffects();
}

// 255 - 255 * (current - start)/(final - start)
// 255 * (current - start)/(final - start)
// all alpha - alpha that should've been lost

SDL_Renderer * GameData::GameRender()
{
	return m_renderer;
}

SDL_Texture * GameData::LoadImage(std::string path)
{
	return LoadTexture(path, m_renderer);
}
/// <summary>
/// Draws to the game.
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
/// Shuts down game.
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
/// <returns></returns>
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

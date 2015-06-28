#include "include.h"

const int SCREEN_WIDTH = 740;
const int SCREEN_HEIGHT = 480;

const int LEVEL_WIDTH = 1280;
const int LEVEL_HEIGHT = 960;

const int TOTAL_TILES = 192;
const int TOTAL_TILE_SPRITES = 12;

TextureData texture1;
TextureData texture2;
TextureData textureText;
TextureData textureArrow;
TextureData textureColorkey;
TextureData textureTiles;
TextureData textureColors;
TextureData texturePixel;
TextureData textureProfile;
TextureData textureLarvesta;
TextureData textureHealth;

//MapData map;
SDL_Rect w;

extern GameData game;

static int elevation;

/*double degrees = 0;
SDL_RendererFlip flipType = SDL_FLIP_NONE;
*/

//SDL_Rect camera = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};

EntityData player;
int p_x;
int p_y;

/*Modulation components
Uint8 r = 255;
Uint8 g = 255;
Uint8 b = 255;*/

/*
Uint8 a = 0;
int final = 5000;
int start = 0;
*/

int current;
bool nextscreen;

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
	m_camera.x = 0;
	m_camera.y = 0;
	m_camera.w = SCREEN_WIDTH;
	m_camera.h = SCREEN_WIDTH;

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
			SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH,
			SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
		
		if (m_window == NULL)
		{
			printf("window failed: %s\n", SDL_GetError() );
			success = false;
		}
		else
		{
			m_renderer = SDL_CreateRenderer (m_window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
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

					if (!texturePixel.Setup(LoadTexture("image/mouse-cursor-pointer.png", m_renderer)))
					{
						printf("load texture pixel fail\n");
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

						m_currentMap = new MapData();

						if (!m_currentMap->Setup("20map.map"))
						{
							printf("failed to load map file :: 20map.map \n");
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
						if(!textureProfile.Setup(LoadTexture("image/charprofile.png", m_renderer)))
						{
							success = false;
						}
						if (!textureLarvesta.Setup(LoadTexture("image/Larvesta.png", m_renderer)))
						{
							success = false;
						}
						if(!textureHealth.Setup(LoadTexture("image/health.png", m_renderer)))
						{
							success = false;
						}
						player.Setup();
						player.AddToTile(0, 0);
						player.SetName("player");
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
				case SDL_MOUSEMOTION:
				{
					/*
					int x;
					int	y;

					SDL_GetMouseState(&x, &y);

					/*if (x >= 0.9*SCREEN_WIDTH)
						camera.x -= 16;
					if (x <= 0.1*SCREEN_WIDTH)
						camera.x += 16;
					if (y >= 0.9*SCREEN_HEIGHT)
						camera.y -= 16;
					if (y <= 0.1 *SCREEN_HEIGHT)
						camera.y += 16;

					//printf("Mouse: %d :: %d\n", x, y);*/
					break;
				}

				case SDL_MOUSEBUTTONDOWN:
				{
					//tilex = 0;
					//tiley = 0;
					/*
					texture1.SetEffect(TEXFX_FADEOUT, 10 + (int)SDL_GetTicks(), 1000 + (int)SDL_GetTicks());
					
					//textureText.Free();
					break; 
					*/

					/*
					if (!map.Save())
						printf("map failed to save");
					else
						printf("map saved\n");
					*/
					
					int mx, my;
					
					SDL_GetMouseState(&mx, &my);
					
					printf("mouse | %i :: %i\n", mx, my);

					// attempt 1					
					/*
					tilex = ( (my/TILE_HEIGHT) + (mx/(2*TILE_WIDTH)) );
					tiley = ( (my/TILE_HEIGHT) - (mx/(2*TILE_WIDTH)) );
					printf("tile | %i :: %i\n\n", tilex, tiley);
					*/
					/*
					w.x = mx;
					w.y = my;
					w.w = 16;
					w.h = 16;
					*/
				}

				case SDL_MOUSEBUTTONUP:
				{
					//map.EditTileType(tilex, tiley, 0);
					break;
				}

				case SDL_KEYDOWN:
				{
					switch (m_events.key.keysym.sym)
					{
						// tile/camera movement
						case SDLK_LEFT:
							game.SetCamera(64, 0);
							break;
						case SDLK_RIGHT:
							game.SetCamera(-64, 0);
							break;
						case SDLK_UP:
							game.SetCamera(0, 32);
							break;
						case SDLK_DOWN:
							game.SetCamera(0, -32);
							break;
						case SDLK_w:
							//player.Move(0,-1);
							player.SetFrame(1, true);
							player.m_x += 2;
							player.m_y -= 1;
							break;
						case SDLK_a:
							//player.Move(-1, 0);
							player.SetFrame(1, false);
							player.m_x -= 2;
							player.m_y -= 1;
							break;
						case SDLK_s:
							//player.Move(0,1);
							player.SetFrame(0, false);
							player.m_x -= 2;
							player.m_y += 1;
							break;
						case SDLK_d:
							//player.Move(1, 0);
							player.SetFrame(0, true);
							player.m_x += 2;
							player.m_y += 1;
							break;
						case SDLK_q:
							//player.Move(-1,-1);
							player.SetFrame(1, true);
							player.m_y -= 1;
							break;
						case SDLK_e:
							//player.Move(1, -1);
							player.SetFrame(0, true);
							player.m_x += 2;
							break;
						case SDLK_z:
							//player.Move(-1,1);
							player.SetFrame(0, false);
							player.m_x -= 2;
							break;
						case SDLK_c:
							//player.Move(1, 1);
							player.SetFrame(0, true);
							player.m_y += 1;
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
	m_Time = SDL_GetTicks();

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
	
	r.x = 0;
	r.y = 0;
	r.w = 450;
	r.h = 50;

	SDL_Rect z;
	z.x = 640;
	z.y = 380;
	z.w = 93;
	z.h = 93;

	SDL_Rect health;
	health.x = 0;
	health.y = 0;
	health.w = 120;
	health.h = 25;

	game.CurrentMap()->Render(m_renderer, 600, 0);
	
	//texture1.Render (m_renderer, NULL, &camera);
	//textureText.Render(m_renderer, NULL, &r, SDL_FLIP_NONE);
	//texturePixel.Render(m_renderer, NULL, &w, SDL_FLIP_NONE);

	
	//textureProfile.Render(m_renderer, NULL, &z, SDL_FLIP_NONE);

	SDL_Texture *profileTex = SDL_CreateTexture(m_renderer, textureProfile.GetFormat(), 
		SDL_TEXTUREACCESS_TARGET, textureProfile.Width(), textureProfile.Height());
	SDL_SetTextureBlendMode(profileTex, SDL_BLENDMODE_BLEND);

	//SDL_SetRenderDrawBlendMode(m_renderer, SDL_BLENDMODE_BLEND);
	//SDL_RenderCopy(m_renderer, textureProfile.Texture(), NULL, &z);
	SDL_SetRenderTarget(m_renderer, NULL);
	SDL_SetRenderDrawBlendMode(m_renderer, SDL_BLENDMODE_BLEND);
	SDL_SetRenderDrawColor(m_renderer, 0x00, 0x00, 0x00, 0xFF);
	//SDL_RendererInfo info;
	//SDL_GetRendererInfo(m_renderer, &info);
	SDL_SetRenderTarget(m_renderer, profileTex);
	textureProfile.Render(m_renderer, NULL, &z, SDL_FLIP_NONE);
	textureLarvesta.Render(m_renderer, NULL, &z, SDL_FLIP_NONE);
	SDL_RenderCopy(m_renderer, profileTex, NULL, &z);
	SDL_RenderFillRect(m_renderer, &z);
	SDL_RenderCopy(m_renderer, profileTex, NULL, &z);
	SDL_SetRenderTarget(m_renderer, NULL);
	

	/*
	//textureHealth.Render(m_renderer, NULL, &health, SDL_FLIP_NONE);
	
	// color modulation
	textureColors.SetColor(r, g, b);
	textureColors.Render(20, 20, m_renderer);

	// rotation and flipping
	textureArrow.NRender(100, 100, m_renderer, NULL, degrees, NULL, flipType);

	// color key
	textureColorkey.Render(100, 200, m_renderer);
	*/

	if (nextscreen)
	{
		texture2.Render(m_renderer, NULL, NULL, SDL_FLIP_NONE);
	}
	
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
/// Shuts down game and its depending libraries, then closes window.
/// </summary>
void GameData::End()
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
		printf("error in setup function\n");
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

Uint32 GameData::CurrentTime()
{
	return m_Time;
}

SDL_Rect GameData::Camera()
{
	return m_camera;
}

MapData * GameData::CurrentMap()
{
	return m_currentMap;
}

void GameData::SetCamera(int x, int y)
{
	m_camera.x += x;
	m_camera.y += y;

}

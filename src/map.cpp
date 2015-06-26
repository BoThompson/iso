#include "include.h"

extern GameData game;
extern SDL_Rect camera;
extern enum Effects;

const int TOTAL_TILE_SPRITES = 12;

/// <summary>
/// Initializes a new instance of the <see cref="MapData"/> class.
/// </summary>
MapData::MapData()
{
	m_tilesheet = TilesheetData();
	//m_filename = new char[ 1024 ];

	m_height = NULL;
	m_width = NULL;
	//m_filename = NULL;

	m_x = 0;
	m_y = 0;
}

/// <summary>
/// Setups the specified file.
/// </summary>
/// <param name="file">The filename containing the map data.</param>
/// <returns>returns true if if successfully done, false if not</returns>
bool MapData::Setup(std::string file)
{
	bool success = true;

	int x = 0;
	int y = 0;
	int tx = 0;
	int ty = 0;

	int height;
	int width;
	int tileType;

	strcpy(m_filename, file.c_str());

	std::ifstream mapFile (file.c_str());

	if (mapFile == NULL)
	{
		printf("unable to load map file\n");
		success = false;
		game.End();
	}

	mapFile >> width;
	if (mapFile.fail())
	{
		printf( "Error loading map: Cannot find map width.\n" );
		success = false;
		game.End();
	}
	m_width = width;

	mapFile >> height;
	if (mapFile.fail())
	{
		printf( "Error loading map: Cannot find map height.\n" );
		success = false;
		game.End();
	}
	m_height = height;

	m_tilesheet.LoadTileSheet("image/tile.png", game.GameRender());
	for (y = 0; y < MAX_MAP_HEIGHT; y++)
		for (x = 0; x < MAX_MAP_WIDTH ; x++)
			m_tileSet[x][y] = new TileData(-1); //empty tile with empty list

	for (y = 0; y < m_height; y++)
	{
		for (x = 0; x < m_width ; x++)
		{
			

			mapFile >> tileType;

			if (mapFile.fail())
			{
			   printf( "Error loading map: Incorrect Format\n" );
			   success = false;
			   break;
			}

			if ((tileType >= -1) && (tileType < TOTAL_TILE_SPRITES))
			{
				m_tileSet[x][y]->SetType(tileType);
			}
			else
			{
				printf( "Error loading map: Invalid tile type found.\n" );
				success = false;
				break;
			}
		}
	}
	
	for (x = 0; x < m_width ; x++)
	{
		for (y = 0; y < m_height; y++)
		{
			short elevation = 0;

			mapFile >> elevation;
			
			if (mapFile.fail())
			{
			   printf( "Error loading height information: Unexpected end of file!\n" );
			   success = false;
			   break;
			}

			m_tileSet[x][y]->SetHeight(elevation);
		}
	}
	
	mapFile.close();
	return success;
}

/// <summary>
/// Renders the specified renderer.
/// </summary>
/// <param name="renderer">The specified renderer.</param>
/// <param name="x">The x coordinate.</param>
/// <param name="y">The y coordinate.</param>
void MapData::Render(SDL_Renderer * renderer, int x, int y)
{
	SDL_Rect r;
	SDL_Rect entRect;
	m_x = x;
	m_y = y;

	r.w = TILE_WIDTH;
	r.h = TILE_HEIGHT;
	r.x = x + game.Camera().x;
	r.y = y + game.Camera().y; // tile y

	float heightChange = 0;

	int rx;

	for (int sy = 0; sy < m_width + m_height; sy++)
	{
		int ty = sy;
		int tx;

		rx = r.x; // saves where we are on the tileset x

		for (tx = 0; ty >= 0; tx++)
		{			
			if(tx < m_width && ty < m_height
			&& m_tileSet[tx][ty]->GetType() != -1)
			{
				heightChange = 0;
				if (m_tileSet[tx][ty]->GetHeight() > 0)
				{
					for(int h = 0; h < m_tileSet[tx][ty]->GetHeight() - 1; h++)
					{
						m_tilesheet.Render(renderer, 5, r); // places collar
						r.y -= (TILE_HEIGHT +1)/2;
						heightChange-= (TILE_HEIGHT +1)/2;
					}
					m_tilesheet.Render(renderer, 4, r);
					r.y -= (TILE_HEIGHT +1)/2;
					heightChange-= (TILE_HEIGHT +1)/2;
				}else if (m_tileSet[tx][ty]->GetHeight() < 0)
				{
					m_tilesheet.Render(renderer, 6, r);
					r.y += (TILE_HEIGHT +1)/2;
					heightChange+= (TILE_HEIGHT +1)/2;
					for(int h = -m_tileSet[tx][ty]->GetHeight() - 1; h > 0 ; h--)
					{
						m_tilesheet.Render(renderer, 7, r); // places collar
						r.y += (TILE_HEIGHT +1)/2;
						heightChange+= (TILE_HEIGHT +1)/2;
					}
				}

				m_tilesheet.Render(renderer, m_tileSet[tx][ty]->GetType(), r); // places tile
				//SDL_RenderPresent(game.GameRender());
				r.y -= heightChange;
			}
			ty--; // moves to next spot "y" on tileset
			r.x += 64; // moves tilesheet clipper to the right
		}
		 ty = sy;
		 r.x = rx;
		for (tx = 0; ty >= 0; tx++)
		{			
			if(tx < m_width && ty < m_height
				&& m_tileSet[tx][ty]->GetType() != -1)
			{
				heightChange = 0;
				if (m_tileSet[tx][ty]->GetHeight() > 0)
				{
					for(int h = 0; h < m_tileSet[tx][ty]->GetHeight() - 1; h++)
					{
						r.y -= (TILE_HEIGHT +1)/2;
						heightChange-= (TILE_HEIGHT +1)/2;
					}	
					r.y -= (TILE_HEIGHT +1)/2;
					heightChange-= (TILE_HEIGHT +1)/2;
				}else if (m_tileSet[tx][ty]->GetHeight() < 0)
				{
		
					r.y += (TILE_HEIGHT +1)/2;
					heightChange+= (TILE_HEIGHT +1)/2;
					for(int h = -m_tileSet[tx][ty]->GetHeight() - 1; h > 0 ; h--)
					{			
						r.y += (TILE_HEIGHT +1)/2;
						heightChange+= (TILE_HEIGHT +1)/2;
					}
				}

				int centerX = r.x + TILE_WIDTH / 4;
				int centerY = r.y - TILE_HEIGHT / 2;

				m_tileSet[tx][ty]->SetBoundaries(centerX, centerY);

				for(EntityData *e = m_tileSet[tx][ty]->OnTile(); e != NULL; e = e->next)
				{
					entRect.y = (r.y - TILE_HEIGHT / 2) + e->m_y; // center of tile
					entRect.x = (r.x + TILE_WIDTH / 4) + e->m_x;// center of tile
					entRect.w = TEXTURE_WIDTH;
					entRect.h = TEXTURE_HEIGHT;
					e->Render(renderer, NULL, entRect);
				}

				r.y -= heightChange;
			}
			ty--; // moves to next spot "y" on tileset
			r.x += 64; // moves tilesheet clipper to the right
		}

		// now start clipping from a new x
		r.x = rx - 32;
		// moves tilesheet clipper down 16
		r.y += 16;
	}
}

bool MapData::Save()
{
	bool status = true;
	FILE * mapFile;

	//take edited data from array and save to file
	if (m_filename != NULL)
	{
		//std::ofstream mapFile(m_filename);
		mapFile = fopen(m_filename, "w");
		if (mapFile == NULL)
		{
			printf("unable to load map file\n");
			status = false;
			game.End();
		}

		//mapFile << m_width + " ";
		//mapFile << m_height + "\n";

		fprintf(mapFile, "%i %i\n", m_width, m_height);

		for (int y = 0; y < m_height; y++)
		{
			fprintf(mapFile, "\n");

			for (int x = 0; x < m_width ; x++)
			{
				//mapFile << m_tileSet[x][y]->m_tileType + " ";
				fprintf(mapFile, "%i ",  m_tileSet[x][y]->GetType());
			}
		}

		//mapFile << "\n \n";
		fprintf(mapFile, "\n \n");

		for (int x = 0; x < m_width ; x++)
		{
			fprintf(mapFile, "\n");

			for (int y = 0; y < m_height; y++)
			{
				//mapFile << m_tileSet[x][y]->m_elevation + " ";
				fprintf(mapFile, "%i ", m_tileSet[x][y]->GetHeight());
			}
		}
		
		//mapFile.close();
		fclose(mapFile);
	}
	else
		status = false;

	return status;
}

bool MapData::Close()
{
	// will close the map
	return true;
}

void MapData::SetType(int tx, int ty, int tileType)
{
	m_tileSet[tx][ty]->SetType(tileType);
}

int MapData::GetHeight(int tx, int ty)
{
	return m_tileSet[tx][ty]->GetHeight();
}

bool MapData::AddEntityToTile(int tx, int ty, EntityData * ent)
{
	if(tx < 0 
	|| ty < 0 
	|| tx >= m_width
	|| ty >= m_height) //TODO : THIS SHOULD BE AN ERROR
		return false;
	m_tileSet[tx][ty]->AddToList(ent);

	ent->SetParent(m_tileSet[tx][ty]);

	return true;
}

bool MapData::RemoveEntityFromTile(int tx, int ty, EntityData * ent)
{
	if(tx < 0 
	|| ty < 0 
	|| tx >= m_width
	|| ty >= m_height) //TODO : THIS SHOULD BE AN ERROR
		return false;

	m_tileSet[tx][ty]->RemoveFromList(ent);

	ent->ClearParent();

	return true;
}

void MapData::PrintOnTile(int tx, int ty)
{
	m_tileSet[tx][ty]->PrintLinkList();
}
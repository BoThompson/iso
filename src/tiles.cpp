#include "include.h"

extern enum Effects;

/// <summary>
/// Initializes a new instance of the <see cref="TileData"/> struct.
/// </summary>
/// <param name="tileType">Type of the tile.</param>
TileData::TileData(int tileType)
{
	m_tileType = tileType;
	m_onTile = NULL; // tiles individual list is set to null when initialized
}

void TileData::SetHeight(short elevation)
{
	m_elevation = elevation;
}

void TileData::SetType(int tileType)
{
	m_tileType = tileType;
}

int TileData::GetHeight ()
{
	return m_elevation;
}

int TileData::GetType()
{
	return m_tileType;
}

/// <summary>
/// Goes to end of list and adds itself
/// or starts off a new list.
/// </summary>
/// <param name="ent">Entity.</param>
void TileData::AddToList(EntityData * ent)
{
	EntityData * n = new EntityData();
	ent->next = NULL;
	
	if (m_onTile != NULL)
	{
		current = m_onTile;

		//list search
		while (current->next != NULL)
		{
			current = current->next;
		}

		current->next = ent;
	}
	else
	{
		m_onTile = ent;
	}
}

/// <summary>
/// Deletes from and patches up list.
/// </summary>
/// <param name="ent">The ent.</param>
EntityData *TileData::RemoveFromList(EntityData * ent)
{
	EntityData *prev;
	bool found = false;
	//First check to see if the head of the list is the entity
	if(ent == m_onTile)
	{
		//If so, remove the head
		m_onTile = m_onTile->next;
	}else{
		//Otherwise cycle through finding the entity
		//Store the head of the list as the "previous"
		prev = m_onTile;
		for(EntityData *ptr = m_onTile; ptr ; ptr = ptr->next)
		{
			if(ptr == ent)
			{
				//We found the entity
				found = true;
				break;
			}else
				prev = ptr;
		}
		//Take him out of the list by connecting his previous to his next
		if(found){
			prev->next = ent->next;
			return ent;
		}else{
			printf("%s was not found in list\n", ent->m_name);
			return NULL;
		}
	}
}

void TileData::PrintLinkList()
{

}

EntityData * TileData::OnTile()
{
	return m_onTile;
}
/// <summary>
/// Initializes a new instance of the <see cref="TilesheetData"/> class.
/// </summary>
TilesheetData::TilesheetData()
{
	m_textureTile = TextureData();
}

/// <summary>
/// Renders the tilesheet to the specified renderer.
/// </summary>
/// <param name="renderer">The renderer.</param>
/// <param name="num">The tile number.</param>
/// <param name="dest">Where it start appearing.</param>
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

/// <summary>
/// Loads the tilesheet from filename, for the specified renderer.
/// </summary>
/// <param name="path">The filename</param>
/// <param name="renderer">The specified renderer</param>
void TilesheetData::LoadTileSheet(std::string path, SDL_Renderer * renderer)
{
	//SDL_Texture * texture = LoadTexture(path, renderer);
	m_textureTile = TextureData();
	m_textureTile.Setup(LoadTexture(path, renderer));
	//TextureData nTexture;

	//if(nTexture.Setup(LoadTexture(path, renderer)))
		//return &nTexture;
	
}
#include "include.h"

extern GameData game;

EntityData::EntityData()
{
	
}

void EntityData::Setup()
{
	m_spritesheet.LoadSheet("image/sheet.png", game.GameRender(), 0, 128, 192);
}

void EntityData::SetName(const char * name)
{
	strcpy(m_name, name);
}

void EntityData::Render(SDL_Renderer * renderer, int num, int x, int y)
{

	SDL_Rect dest;

	dest.x = m_x;
	dest.y = m_y;
	dest.w = TEXTURE_WIDTH;
	dest.h = TEXTURE_HEIGHT;

	// tilex = trunc( (offsetY/TILE_HEIGHT) + (offsetX/(TILE_WIDTH)) );
	// tiley = trunc( (offsetY/TILE_HEIGHT) - (offsetX/(TILE_WIDTH)) );

	 //m_spritesheet.Render(renderer, 1, dest); // debug
}

void EntityData::Render(SDL_Renderer * renderer, SDL_Rect r)
{
	m_spritesheet.Render(renderer, NULL, r);
}

void EntityData::Move(int x, int y)
{
	// should only happen when i reached limit of tile i am on

	game.CurrentMap()->RemoveEntityFromTile(m_tx, m_ty, this);
	AddToTile(m_tx + x, m_ty + y);
}

void EntityData::AddToTile(int tx, int ty)
{
	if(game.CurrentMap()->AddEntityToTile(tx, ty, this))
	{
		m_tx = tx;
		m_ty = ty;
	}
}

void EntityData::SetParent(TileData * tile)
{
	m_parentTile = tile;
}

void EntityData::RemoveFromTile()
{
	if(game.CurrentMap()->RemoveEntityFromTile(m_tx, m_ty, this))
	{
		m_tx = -1;
		m_ty = -1;
	}
}

void EntityData::ClearParent()
{
	m_parentTile = NULL;
}
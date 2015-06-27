#include "include.h"

extern GameData game;

EntityData::EntityData()
{
	
}

void EntityData::Setup()
{
	m_spritesheet.LoadSheet("image/sheet.png", game.GameRender(), 0, 128, 192);
	this->m_animator = new AnimatorData("anim/player.anim");
}

void EntityData::SetName(const char * name)
{
	strcpy(m_name, name);
}

/*
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
*/
void EntityData::Render(SDL_Renderer * renderer, int num, SDL_Rect r)
{
	//m_spritesheet.Render(renderer, this->frame, r, this->flipType);
	this->m_animator->Animate(m_spritesheet, &m_animator->m_animations[0], r, this->flipType);

	if (m_parentTile != NULL && m_parentTile->OnTile() != NULL)
	{
		this->currentcenterX = m_parentTile->CenterX();
		this->currentcenterY = m_parentTile->CenterY();

		this->currentleftBound = m_parentTile->LeftBound();
		this->currentrightBound = m_parentTile->RightBound();
		this->currenttopBound = m_parentTile->TopBound();
		this->currentbottomBound = m_parentTile->BottomBound();
	}

	if (r.y < currenttopBound)
	{
		if (r.x + 10 > currentrightBound) // if closer to right bound
		{
			printf("outside on top right, tile change +0, +1");
			Move(0,-1);
			m_x = m_x - 32;
			m_y = 0;
		}
		else if (r.x - 10 < currentleftBound) // if closer to left bound
		{
			printf("outside on top left, tile change +0, -1");
			Move(-1, 0);
			m_x = 0;
			m_y = 0;
		}
		else if (r.y + 8 > currenttopBound)
		{
			printf("move directly up, tile change +1, +0");
			Move(-1, -1); //looks a little weird...
			m_x = 0;
			m_y = 0;
		}
	}

	if (r.y > currentbottomBound)
	{
		if (r.x + 10 > currentrightBound) // if closer to right bound
		{
			printf("outside on bottom right, tile change +0, +1");
			Move(1,0);
			m_x = 32 - m_x;
			m_y = -1;
		}
		else if (r.x - 10 < currentleftBound) // if closer to left bound
		{
			printf("outside on bottom left, tile change +1, +0");
			Move(0, 1); // shouldn't this be 1, 0?
			m_x = 0;
			m_y = 1;
		}
		else if (r.y - 10 > currentbottomBound)
		{
			printf("move directly down, tile change +1, +0");
			Move(1, 1);
			m_x = 0;
			m_y = 1;
		}
	}
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

void EntityData::SetFrame(int frame, bool flip)
{
	if (frame == 0)
	{
		switch (flip)
		{
		case false:
			this->flipType = SDL_FLIP_NONE;
			this->frame = frame;
			break;
		case true:
			this->flipType = SDL_FLIP_HORIZONTAL;
			this->frame = frame;
			break;
		}
	}

	if (frame == 1)
	{
		switch (flip)
		{
		case false:
			this->flipType = SDL_FLIP_NONE;
			this->frame = frame;
			break;
		case true:
			this->flipType = SDL_FLIP_HORIZONTAL;
			this->frame = frame;
			break;
		}
	}
	
}
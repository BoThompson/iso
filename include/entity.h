#pragma once
#ifndef _PLAYER_H_
#define _PLAYER_H_

class EntityData //entity.h/cpp
{
	int		m_tx;
	int		m_ty; // tileset x and y
	float	m_height;
	float	m_width;
	TextureSheetData m_spritesheet;
	int		frame;
	SDL_RendererFlip	flipType;
	bool	m_isAlreadyFlipped;
	struct TileData	* m_parentTile; // avoid circular dependency
	AnimatorData * m_animator;

	/* The following values will come from the current tile we are on */
	int currentcenterX;
	int currentcenterY;

	int currentleftBound;
	int currentrightBound;
	int currenttopBound;
	int currentbottomBound;

public:
	int		m_x;
	int		m_y;

	EntityData();
	
	void Setup();
	void SetName(const char * name);
	//void Render(SDL_Renderer * renderer, int num, int x, int y);
	void Render(SDL_Renderer * renderer, int num, SDL_Rect r);
	
	void Move(int x, int y);
	
	void AddToTile(int tx, int ty);
	void SetParent (TileData * tile);
	void RemoveFromTile();
	void ClearParent();

	void SetFrame(int frame, bool flip);
	
	EntityData *next; // take player out of old list, put in new list
	char m_name[25];
};

#endif
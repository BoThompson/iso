#include <stdio.h>
#include <string>
#include <stdlib.h>
#include <SDL.h>
#include <SDL_image.h>
#include "utils.h"
#include "game.h"
#include "texture.h"

extern enum Effects; // extern is ignored
extern bool nextscreen;

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

/// <summary>
/// Sets the alpha.
/// </summary>
/// <param name="alpha">The alpha rate requested.</param>
void TextureData::SetAlpha(Uint8 alpha)
{
	SDL_SetTextureAlphaMod(m_texture, alpha);
}


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
				SetAlpha( a ); // change animation # frame = (currentTime - start)/(end - start)
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

void TextureSheetData::LoadSheet(std::string path, SDL_Renderer * renderer, Uint8 r, Uint8 g, Uint8 b)
{
	m_texture = TextureData();
	m_texture.Setup(LoadTexture(path, renderer, r, g, b));
}

void TextureSheetData::Render(SDL_Renderer * renderer, int num, SDL_Rect dest)
{
	SDL_Rect source;

	source.x = (num * TEXTURE_WIDTH) % 32;
	source.y = (num * TEXTURE_WIDTH) / 32 * TEXTURE_HEIGHT;
	source.w = TEXTURE_WIDTH;
	source.h = TEXTURE_HEIGHT;

	m_texture.Render(renderer, &source, &dest);
}
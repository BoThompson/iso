#include <SDL.h>
#include <SDL_image.h>
#include <string>
#include "utils.h"
#include "game.h"
#include "texture.h"
#include <cmath>

/// <summary>
/// Loads the texture.
/// </summary>
/// <param name="path">The filepath.</param>
/// <param name="renderer">SDL Renderer.</param>
/// <returns> returns a SDL Texture or NULL if failed</returns>
SDL_Texture * LoadTexture (std ::string path, SDL_Renderer * renderer )
{
	SDL_Texture * finalTexture = NULL;

	SDL_Surface * loadedSurface = IMG_Load(path.c_str() );

	if (loadedSurface == NULL)
	{
		printf("load image fail: %s\n", IMG_GetError() );
	}
	else
	{
		SDL_SetColorKey(loadedSurface, SDL_TRUE, SDL_MapRGB(loadedSurface->format, 0, 0xFF, 0xFF));
		finalTexture = SDL_CreateTextureFromSurface(renderer, loadedSurface);
		if (finalTexture == NULL)
		{
			printf("cannot render texture", SDL_GetError());
		}

		SDL_FreeSurface (loadedSurface);
	}

	return finalTexture;
}

/// <summary>
/// Overloaded function that loads the texture.
/// </summary>
/// <param name="text">The text to display.</param>
/// <param name="font">The font to display the text with.</param>
/// <param name="color">The color.</param>
/// <param name="renderer">The specified renderer.</param>
/// <returns>returns the final version of the texture</returns>
SDL_Texture * LoadTexture (std ::string text, TTF_Font * font, SDL_Color color, SDL_Renderer * renderer )
{
	SDL_Surface * textSurface = TTF_RenderText_Solid( font, text.c_str(), color);
	SDL_Texture * finalTexture = NULL;
	if (textSurface == NULL)
	{
		printf("unable to render text surface: %s\n", TTF_GetError() );
	}
	else
	{
		finalTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
		if (finalTexture == NULL)
		{
			printf("unable to create texture: %s\n", SDL_GetError() );
		}
		
		SDL_FreeSurface(textSurface);
	}

	return finalTexture;
}

SDL_Texture * LoadTexture (std ::string path, SDL_Renderer * renderer, Uint8 r, Uint8 g, Uint8 b )
{
	SDL_Texture * finalTexture = NULL;

	SDL_Surface * loadedSurface = IMG_Load(path.c_str() );

	if (loadedSurface == NULL)
	{
		printf("load image fail: %s\n", IMG_GetError() );
	}
	else
	{
		SDL_SetColorKey(loadedSurface, SDL_TRUE, SDL_MapRGB(loadedSurface->format, r, g, b));
		finalTexture = SDL_CreateTextureFromSurface(renderer, loadedSurface);
		if (finalTexture == NULL)
		{
			printf("cannot render texture", SDL_GetError());
		}

		SDL_FreeSurface (loadedSurface);
	}

	return finalTexture;
}

/// <summary>
/// Setups the specified texture from raw texture data.
/// </summary>
/// <param name="texture">Raw texture data.</param>
/// <returns>returns true if done, false if texture given is null or if texture already setup</returns>
bool TextureData::Setup (SDL_Texture * texture)
{
	if (texture == NULL || m_texture != NULL)
		return false;

	SDL_QueryTexture(texture, NULL, NULL, &m_width, &m_height);

	m_texture = texture;

	return true;
}

/*
bool TextureSheetData::Setup(SDL_Texture * texture)
{
	if (texture == NULL || m_texture != NULL)
		return false;

	SDL_QueryTexture(texture, NULL, NULL, &m_width, &m_height);

	m_texture = texture;

	return true;
}
*/

/// <summary>
/// Frees this instance from memory.
/// </summary>
void TextureData::Free()
{
	if( m_texture != NULL )
	{
		SDL_DestroyTexture( m_texture );
		m_texture = NULL;
		m_width = 0;
		m_height = 0;
	}
}

double trunc(double d)
{
	return (d>0) ? floor(d) : ceil(d);
}
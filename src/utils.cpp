#include <SDL.h>
#include <SDL_image.h>
#include <string>
#include "utils.h"
#include "game.h"

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

bool TextureData::Setup (SDL_Texture * texture)
{
	if (texture == NULL || m_texture != NULL)
		return false;

	SDL_QueryTexture(texture, NULL, NULL, &m_width, &m_height);

	m_texture = texture;

	return true;
}

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
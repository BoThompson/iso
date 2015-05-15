#include <SDL.h>
#include <SDL_image.h>
#include <string>

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
		finalTexture = SDL_CreateTextureFromSurface(renderer, loadedSurface);
		if (finalTexture == NULL)
		{
			printf("cannot render texture", SDL_GetError());
		}

		SDL_FreeSurface (loadedSurface);
	}

	return finalTexture;
}
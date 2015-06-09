#ifndef _TEXTURE_H_
#define _TEXTURE_H_


/// <summary>
/// TextureData Class
/// </summary>
class TextureData //texture.h/cpp
{
	/// <summary>
	/// The raw texture data.
	/// </summary>
	SDL_Texture * m_texture;
	/// <summary>
	/// The width of the texture
	/// </summary>
	int m_width;
	/// <summary>
	/// The height of the texture
	/// </summary>
	int m_height;
	/// <summary>
	/// The bit flags, gives ability for multiple effects at once.
	/// </summary>
	int m_flags;
	/// <summary>
	/// The start time for the designated effect
	/// </summary>
	long m_texfxStart[2]; //error
	/// <summary>
	/// The end time for designated effect
	/// </summary>
	long m_texfxEnd[2];

public:
	TextureData();
	//~TextureData();

	bool Setup (SDL_Texture * texture);
	void Free();
	void SetColor(Uint8 red, Uint8 green, Uint8 blue);
	void SetBlendMode(SDL_BlendMode blending);
	void SetAlpha(Uint8 alpha);
	void Render (SDL_Renderer * renderer, SDL_Rect * src, SDL_Rect * dest);
	void NRender (int x, int y, SDL_Renderer * renderer, SDL_Rect * clip = NULL, double angle = 0.0, SDL_Point * center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE );
	void SetEffect(int flag, long start, long final);
	void UpdateEffects();
	int Width();
	int Height();
	int SetWidth(int width);
	int SetHeight(int height);
};

#endif
#ifndef _UTILS_H_
#define _UTILS_H_

#include <SDL_ttf.h>

#define SET_FLAG(a,b)	a|=b
#define REMOVE_FLAG(a,b) a &= ~b
#define IS_SET(a,b) (a & b)

#define SET_BFLAG(a,b)	a |= 1<<b
#define REMOVE_BFLAG(a, b) a &= ~(1<<b)
#define IS_BSET(a, b) (a & (1<<b))

SDL_Texture * LoadTexture (std ::string path, SDL_Renderer * renderer );
SDL_Texture * LoadTexture (std ::string path, SDL_Renderer * renderer, Uint8 r, Uint8 g, Uint8 b );
SDL_Texture * LoadTexture (std ::string text, TTF_Font * font, SDL_Color color, SDL_Renderer * renderer );
SDL_Texture * LoadTexture (std ::string path, SDL_Renderer * renderer, Uint8 r, Uint8 g, Uint8 b, bool srccolorkey = false );

double trunc (double d);

#endif _UTILS_H_
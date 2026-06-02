#pragma once
#include <SDL3/SDL.h>

SDL_Texture *sprite_load(SDL_Renderer *renderer, const char *path);
void         sprite_destroy(SDL_Texture *texture);

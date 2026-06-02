#include "sprite.h"

SDL_Texture *sprite_load(SDL_Renderer *renderer, const char *path) {
    SDL_Surface *surface = SDL_LoadBMP(path);
    if (!surface) {
        SDL_Log("sprite_load: %s", SDL_GetError());
        return NULL;
    }
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_DestroySurface(surface);
    if (!texture)
        SDL_Log("sprite_load texture: %s", SDL_GetError());
    return texture;
}

void sprite_destroy(SDL_Texture *texture) {
    if (texture) SDL_DestroyTexture(texture);
}

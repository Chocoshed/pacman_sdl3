#include "renderer.h"
#include "sprite_coords.h"

static void blit(SDL_Renderer *r, SDL_Texture *sheet,
                 const SDL_FRect *src, float dx, float dy, float dw, float dh) {
    SDL_FRect dst = { dx, dy, dw, dh };
    SDL_RenderTexture(r, sheet, src, &dst);
}

void draw_hud(SDL_Renderer *renderer, SDL_Texture *sheet, const Score *score) {
    /* Ligne 0 : vies — sprite bouche fermée de Pac-Man */
    for (int i = 0; i < score->lives; i++)
        blit(renderer, sheet, &PACMAN[0], 4.0f + (float)i * 16.0f, 2.0f, 12.0f, 12.0f);

    /* Ligne 1 : barre de score proportionnelle au record */
    int max_score = (score->high_score > 0) ? score->high_score : 10000;
    int bar_w     = (score->score * 448) / max_score;
    if (bar_w > 448) bar_w = 448;
    if (bar_w > 0) {
        SDL_FRect bar = { 0.0f, 18.0f, (float)bar_w, 12.0f };
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderFillRect(renderer, &bar);
    }

    /* Ligne 2 : indicateurs de niveau (carrés cyan) */
    SDL_SetRenderDrawColor(renderer, 0, 255, 255, 255);
    for (int i = 0; i < score->level && i < 20; i++) {
        SDL_FRect rect = { 4.0f + (float)i * 16.0f, 34.0f, 12.0f, 12.0f };
        SDL_RenderFillRect(renderer, &rect);
    }
}

#include "renderer.h"
#include "sprite_coords.h"

static void blit(SDL_Renderer *r, SDL_Texture *sheet,
                 const SDL_FRect *src, float dx, float dy, float dw, float dh) {
    SDL_FRect dst = { dx, dy, dw, dh };
    SDL_RenderTexture(r, sheet, src, &dst);
}

void draw_hud(SDL_Renderer *renderer, SDL_Texture *sheet, const Score *score) {
    /* Barre de score proportionnelle au record */
    int max_score = (score->high_score > 0) ? score->high_score : 10000;
    int bar_w     = (score->score * 448) / max_score;
    if (bar_w > 448) bar_w = 448;
    if (bar_w > 0) {
        SDL_FRect bar = { 0.0f, 18.0f, (float)bar_w, 12.0f };
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderFillRect(renderer, &bar);
    }

    /* Indicateurs de niveau (carrés cyan) */
    SDL_SetRenderDrawColor(renderer, 0, 255, 255, 255);
    for (int i = 0; i < score->level && i < 20; i++) {
        SDL_FRect rect = { 4.0f + (float)i * 16.0f, 34.0f, 12.0f, 12.0f };
        SDL_RenderFillRect(renderer, &rect);
    }

    /* Vies — bas-gauche, sprite bouche mi-ouverte */
    float bottom_y = (float)((MAZE_ROWS - 1) * CELL_SIZE) + 1.0f;
    for (int i = 0; i < score->lives; i++)
        blit(renderer, sheet, &PACMAN[1], 4.0f + (float)i * 16.0f, bottom_y, 14.0f, 14.0f);

    /* 7 derniers fruits collectés — bas-droite, du plus ancien (gauche) au plus récent (droite) */
    float fx_right = (float)(MAZE_COLS * CELL_SIZE) - 4.0f;
    for (int i = 0; i < score->fruit_history_count; i++) {
        float x = fx_right - (float)(score->fruit_history_count - i) * 16.0f;
        blit(renderer, sheet, &FRUIT_SPRITES[score->fruit_history[i]], x, bottom_y, 14.0f, 14.0f);
    }
}

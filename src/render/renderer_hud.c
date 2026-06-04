#include "renderer.h"
#include "sprite_coords.h"

static void blit(SDL_Renderer *r, SDL_Texture *sheet,
                 const SDL_FRect *src, float dx, float dy, float dw, float dh) {
    SDL_FRect dst = { dx, dy, dw, dh };
    SDL_RenderTexture(r, sheet, src, &dst);
}

static void draw_number(SDL_Renderer *renderer, SDL_Texture *sheet,
                        int value, float x, float y) {
    int digits[10];
    int count = 0;
    if (value == 0) {
        digits[count++] = 0;
    } else {
        int v = value;
        while (v > 0) { digits[count++] = v % 10; v /= 10; }
        for (int i = 0, j = count - 1; i < j; i++, j--)
            { int t = digits[i]; digits[i] = digits[j]; digits[j] = t; }
    }
    for (int i = 0; i < count; i++)
        blit(renderer, sheet, &GLYPH_DIGIT[digits[i]],
             x + (float)i * 9.0f, y, 8.0f, 8.0f);
}

void draw_hud(SDL_Renderer *renderer, SDL_Texture *sheet, const Score *score) {
    /* Score — haut-gauche, chiffres 8×8 */
    draw_number(renderer, sheet, score->score, 4.0f, 4.0f);

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

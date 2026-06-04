#include "renderer.h"
#include "sprite_coords.h"

static void blit(SDL_Renderer *r, SDL_Texture *sheet,
                 const SDL_FRect *src, float dx, float dy, float dw, float dh) {
    SDL_FRect dst = { dx, dy, dw, dh };
    SDL_RenderTexture(r, sheet, src, &dst);
}

/* Dessine un entier ≥ 0 en sprites chiffres 8×8, sans zéros de tête. */
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

/* Retourne la largeur en pixels d'un entier rendu par draw_number. */
static float number_width(int value) {
    if (value == 0) return 8.0f;
    int count = 0;
    while (value > 0) { count++; value /= 10; }
    return (float)(count * 9 - 1);
}

/* Dessine une chaîne en majuscules (A-Z) et chiffres (0-9), glyphes 8×8. */
static void draw_text(SDL_Renderer *renderer, SDL_Texture *sheet,
                      const char *text, float x, float y) {
    for (int i = 0; text[i]; i++) {
        char c = text[i];
        if (c >= 'A' && c <= 'Z') {
            blit(renderer, sheet, &GLYPH_LETTER[c - 'A'], x, y, 8.0f, 8.0f);
            x += 9.0f;
        } else if (c >= '0' && c <= '9') {
            blit(renderer, sheet, &GLYPH_DIGIT[c - '0'], x, y, 8.0f, 8.0f);
            x += 9.0f;
        } else {
            x += 8.0f; /* espace */
        }
    }
}

/* Retourne la largeur en pixels d'une chaîne rendue par draw_text. */
static float text_width(const char *text) {
    float w = 0.0f;
    for (int i = 0; text[i]; i++)
        w += (text[i] == ' ') ? 8.0f : 9.0f;
    if (w >= 1.0f) w -= 1.0f; /* pas de gap après le dernier glyphe */
    return w;
}

#define WINDOW_W ((float)(MAZE_COLS * CELL_SIZE))

void draw_hud(SDL_Renderer *renderer, SDL_Texture *sheet, const Score *score) {
    /* --- Ligne 1 (y=2) : label "HIGH SCORE" centré --- */
    const char *label   = "HIGH SCORE";
    float       lw      = text_width(label);
    draw_text(renderer, sheet, label, (WINDOW_W - lw) / 2.0f, 2.0f);

    /* --- Ligne 2 (y=12) : score actuel (gauche) et high score (centré) --- */
    draw_number(renderer, sheet, score->score, 4.0f, 12.0f);

    float hw = number_width(score->high_score);
    draw_number(renderer, sheet, score->high_score,
                (WINDOW_W - hw) / 2.0f, 12.0f);

    /* --- Indicateurs de niveau (carrés cyan, y=28) --- */
    SDL_SetRenderDrawColor(renderer, 0, 255, 255, 255);
    for (int i = 0; i < score->level && i < 20; i++) {
        SDL_FRect rect = { 4.0f + (float)i * 16.0f, 28.0f, 12.0f, 12.0f };
        SDL_RenderFillRect(renderer, &rect);
    }

    /* --- Vies — bas-gauche, sprite bouche mi-ouverte --- */
    float bottom_y = (float)((MAZE_ROWS - 1) * CELL_SIZE) + 1.0f;
    for (int i = 0; i < score->lives; i++)
        blit(renderer, sheet, &PACMAN[1], 4.0f + (float)i * 16.0f, bottom_y, 14.0f, 14.0f);

    /* --- 7 derniers fruits — bas-droite, du plus ancien (gauche) au plus récent (droite) --- */
    float fx_right = WINDOW_W - 4.0f;
    for (int i = 0; i < score->fruit_history_count; i++) {
        float x = fx_right - (float)(score->fruit_history_count - i) * 16.0f;
        blit(renderer, sheet, &FRUIT_SPRITES[score->fruit_history[i]], x, bottom_y, 14.0f, 14.0f);
    }
}

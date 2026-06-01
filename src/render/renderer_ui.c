#include "renderer.h"

void draw_title(SDL_Renderer *renderer) {
    SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
    SDL_RenderDebugTextFormat(renderer, 196.0f, 180.0f, "PAC-MAN");
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderDebugTextFormat(renderer, 144.0f, 300.0f, "PRESS ENTER TO START");
}

void draw_menu(SDL_Renderer *renderer, int selected_option) {
    static const char *OPTIONS[]  = { "NEW GAME", "QUIT" };
    static const int   OPTION_COUNT = 2;

    SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
    SDL_RenderDebugTextFormat(renderer, 196.0f, 150.0f, "PAC-MAN");

    for (int i = 0; i < OPTION_COUNT; i++) {
        float y = 260.0f + (float)i * 40.0f;
        if (i == selected_option) {
            SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
            SDL_RenderDebugTextFormat(renderer, 172.0f, y, ">");
        } else {
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        }
        SDL_RenderDebugTextFormat(renderer, 188.0f, y, OPTIONS[i]);
    }
}

void draw_pause(SDL_Renderer *renderer) {
    SDL_FRect overlay = { 0.0f, 250.0f, 448.0f, 76.0f };
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 210);
    SDL_RenderFillRect(renderer, &overlay);
    SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
    SDL_RenderDebugTextFormat(renderer, 200.0f, 280.0f, "PAUSE");
    SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);
    SDL_RenderDebugTextFormat(renderer, 176.0f, 304.0f, "P : REPRENDRE");
}

void draw_game_over(SDL_Renderer *renderer, const Score *score) {
    SDL_FRect overlay = { 0.0f, 200.0f, 448.0f, 200.0f };
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 220);
    SDL_RenderFillRect(renderer, &overlay);

    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_RenderDebugTextFormat(renderer, 188.0f, 220.0f, "GAME OVER");

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderDebugTextFormat(renderer, 160.0f, 260.0f, "SCORE : %d",    score->score);
    SDL_RenderDebugTextFormat(renderer, 160.0f, 284.0f, "MEILLEUR : %d", score->high_score);

    SDL_SetRenderDrawColor(renderer, 180, 180, 180, 255);
    SDL_RenderDebugTextFormat(renderer, 136.0f, 360.0f, "ENTREE : RETOUR AU MENU");
}

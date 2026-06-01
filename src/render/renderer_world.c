#include "renderer.h"

void draw_maze(SDL_Renderer *renderer, const Maze *maze) {
    for (int row = 0; row < MAZE_ROWS; row++) {
        for (int col = 0; col < MAZE_COLS; col++) {
            SDL_FRect rect = {
                .x = (float)(col * CELL_SIZE),
                .y = (float)(row * CELL_SIZE),
                .w = (float)CELL_SIZE,
                .h = (float)CELL_SIZE,
            };

            switch ((CellType)maze->cells[row][col]) {
                case CELL_WALL:
                    SDL_SetRenderDrawColor(renderer, 33, 33, 222, 255);
                    SDL_RenderFillRect(renderer, &rect);
                    break;

                case CELL_DOT: {
                    float cx  = rect.x + CELL_SIZE / 2.0f;
                    float cy  = rect.y + CELL_SIZE / 2.0f;
                    SDL_FRect dot = { cx - 2.0f, cy - 2.0f, 4.0f, 4.0f };
                    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
                    SDL_RenderFillRect(renderer, &dot);
                    break;
                }

                case CELL_POWER_PELLET: {
                    float cx     = rect.x + CELL_SIZE / 2.0f;
                    float cy     = rect.y + CELL_SIZE / 2.0f;
                    SDL_FRect pellet = { cx - 5.0f, cy - 5.0f, 10.0f, 10.0f };
                    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
                    SDL_RenderFillRect(renderer, &pellet);
                    break;
                }

                case CELL_GHOST_DOOR:
                    SDL_SetRenderDrawColor(renderer, 255, 182, 193, 255);
                    SDL_RenderFillRect(renderer, &rect);
                    break;

                case CELL_EMPTY:
                case CELL_FRUIT_SPAWN:
                default:
                    break;
            }
        }
    }
}

void draw_pacman(SDL_Renderer *renderer, const Pacman *pacman) {
    SDL_FRect rect = {
        .x = (float)(pacman->col * CELL_SIZE),
        .y = (float)(pacman->row * CELL_SIZE),
        .w = (float)CELL_SIZE,
        .h = (float)CELL_SIZE,
    };
    SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
    SDL_RenderFillRect(renderer, &rect);
}

void draw_ghost(SDL_Renderer *renderer, const Ghost *ghost) {
    SDL_FRect rect = {
        .x = (float)(ghost->col * CELL_SIZE),
        .y = (float)(ghost->row * CELL_SIZE),
        .w = (float)CELL_SIZE,
        .h = (float)CELL_SIZE,
    };

    switch (ghost->mode) {
        case GHOST_FRIGHTENED:
            SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
            break;
        case GHOST_EATEN:
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            break;
        default:
            switch (ghost->id) {
                case GHOST_BLINKY: SDL_SetRenderDrawColor(renderer, 255,   0,   0, 255); break;
                case GHOST_PINKY:  SDL_SetRenderDrawColor(renderer, 255, 184, 255, 255); break;
                case GHOST_INKY:   SDL_SetRenderDrawColor(renderer,   0, 255, 255, 255); break;
                case GHOST_CLYDE:  SDL_SetRenderDrawColor(renderer, 255, 184,  82, 255); break;
            }
            break;
    }
    SDL_RenderFillRect(renderer, &rect);
}

void draw_fruit(SDL_Renderer *renderer, const Maze *maze, int level) {
    if (!maze->fruit_active)
        return;

    static const SDL_Color FRUIT_COLORS[FRUIT_TYPES] = {
        {220,  20,  60, 255},  /* cerise   */
        {255,  50, 100, 255},  /* fraise   */
        {255, 165,   0, 255},  /* orange   */
        {100, 200,  50, 255},  /* pomme    */
        {100, 220, 100, 255},  /* melon    */
        { 50,  50, 220, 255},  /* galaxian */
        {255, 215,   0, 255},  /* cloche   */
        {200, 200, 200, 255},  /* clé      */
    };

    int        idx = (level - 1 < FRUIT_TYPES) ? level - 1 : FRUIT_TYPES - 1;
    SDL_Color  c   = FRUIT_COLORS[idx];
    SDL_FRect  rect = {
        .x = FRUIT_COL * CELL_SIZE + 2.0f,
        .y = FRUIT_ROW * CELL_SIZE + 2.0f,
        .w = 12.0f,
        .h = 12.0f,
    };
    SDL_SetRenderDrawColor(renderer, c.r, c.g, c.b, c.a);
    SDL_RenderFillRect(renderer, &rect);
}

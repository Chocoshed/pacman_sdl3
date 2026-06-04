#include "renderer.h"
#include "sprite_coords.h"

static void blit(SDL_Renderer *r, SDL_Texture *sheet,
                 const SDL_FRect *src, float dx, float dy, float dw, float dh) {
    SDL_FRect dst = { dx, dy, dw, dh };
    SDL_RenderTexture(r, sheet, src, &dst);
}

void draw_maze(SDL_Renderer *renderer, SDL_Texture *sheet, const Maze *maze) {
    for (int row = 0; row < MAZE_ROWS; row++) {
        for (int col = 0; col < MAZE_COLS; col++) {
            float     x    = (float)(col * CELL_SIZE);
            float     y    = (float)(row * CELL_SIZE);
            SDL_FRect rect = { x, y, (float)CELL_SIZE, (float)CELL_SIZE };

            switch ((CellType)maze->cells[row][col]) {
                case CELL_WALL:
                    SDL_SetRenderDrawColor(renderer, 33, 33, 222, 255);
                    SDL_RenderFillRect(renderer, &rect);
                    break;

                case CELL_DOT:
                    blit(renderer, sheet, &TILE_DOT,
                         x, y, (float)CELL_SIZE, (float)CELL_SIZE);
                    break;

                case CELL_POWER_PELLET:
                    blit(renderer, sheet, &TILE_POWER_PELLET,
                         x, y, (float)CELL_SIZE, (float)CELL_SIZE);
                    break;

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

void draw_pacman(SDL_Renderer *renderer, SDL_Texture *sheet, const Pacman *pacman) {
    const SDL_FRect *src = &PACMAN[pacman->anim_frame];
    SDL_FRect dst = {
        (float)(pacman->col * CELL_SIZE),
        (float)(pacman->row * CELL_SIZE),
        (float)CELL_SIZE, (float)CELL_SIZE,
    };
    if (pacman->dir_current == DIR_NONE) {
        SDL_RenderTexture(renderer, sheet, &PACMAN[0], &dst);
    } else {
        SDL_RenderTextureRotated(renderer, sheet, src, &dst,
                                 PACMAN_ANGLE[pacman->dir_current], NULL, SDL_FLIP_NONE);
    }
}

void draw_pacman_death(SDL_Renderer *renderer, SDL_Texture *sheet,
                       const Pacman *pacman, int frame) {
    SDL_FRect dst = {
        (float)(pacman->col * CELL_SIZE),
        (float)(pacman->row * CELL_SIZE),
        (float)CELL_SIZE, (float)CELL_SIZE,
    };
    SDL_RenderTexture(renderer, sheet, &PACMAN_DEATH[frame], &dst);
}

void draw_ghost(SDL_Renderer *renderer, SDL_Texture *sheet, const Ghost *ghost) {
    float            x   = (float)(ghost->col * CELL_SIZE);
    float            y   = (float)(ghost->row * CELL_SIZE);
    const SDL_FRect *src = NULL;

    switch (ghost->mode) {
        case GHOST_FRIGHTENED:
            if (ghost->frightened_timer <= 2.0f)
                src = &SPRITE_FRIGHTENED_FLASH[ghost->anim_frame];
            else
                src = &SPRITE_FRIGHTENED[ghost->anim_frame];
            break;

        case GHOST_EATEN: {
            Direction d = (ghost->dir == DIR_NONE) ? DIR_RIGHT : ghost->dir;
            src = &SPRITE_EATEN[d];
            break;
        }

        default: {
            Direction d = (ghost->dir == DIR_NONE) ? DIR_RIGHT : ghost->dir;
            src = &GHOST_SPRITES[ghost->id][d][ghost->anim_frame];
            break;
        }
    }

    blit(renderer, sheet, src, x, y, (float)CELL_SIZE, (float)CELL_SIZE);
}

void draw_fruit(SDL_Renderer *renderer, SDL_Texture *sheet,
                const Maze *maze, int level) {
    if (!maze->fruit_active)
        return;
    int idx = (level - 1 < 8) ? level - 1 : 7;
    blit(renderer, sheet, &FRUIT_SPRITES[idx],
         (float)(FRUIT_COL * CELL_SIZE), (float)(FRUIT_ROW * CELL_SIZE),
         (float)CELL_SIZE, (float)CELL_SIZE);
}

void draw_score_popups(SDL_Renderer *renderer, SDL_Texture *sheet,
                       const ScorePopup *popups, int count) {
    for (int i = 0; i < count; i++) {
        if (popups[i].timer <= 0.0f)
            continue;
        int idx = popups[i].sprite_idx;
        if (idx < 0 || idx >= SCORE_POPUP_COUNT)
            idx = 0;
        blit(renderer, sheet, &SCORE_POPUP[idx],
             (float)(popups[i].col * CELL_SIZE),
             (float)(popups[i].row * CELL_SIZE),
             (float)CELL_SIZE, (float)CELL_SIZE);
    }
}

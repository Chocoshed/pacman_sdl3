#pragma once

#include <SDL3/SDL.h>
#include "../game/maze.h"
#include "../game/pacman.h"
#include "../game/score.h"
#include "../game/ghost.h"

void draw_maze        (SDL_Renderer *r, SDL_Texture *sheet, const Maze   *maze);
void draw_pacman      (SDL_Renderer *r, SDL_Texture *sheet, const Pacman *pacman);
void draw_pacman_death(SDL_Renderer *r, SDL_Texture *sheet, const Pacman *pacman, int frame);
void draw_ghost       (SDL_Renderer *r, SDL_Texture *sheet, const Ghost  *ghost);
void draw_hud         (SDL_Renderer *r, SDL_Texture *sheet, const Score  *score);
void draw_fruit       (SDL_Renderer *r, SDL_Texture *sheet, const Maze   *maze, int level);
void draw_title       (SDL_Renderer *r);
void draw_menu        (SDL_Renderer *r, int selected_option);
void draw_pause       (SDL_Renderer *r);
void draw_game_over   (SDL_Renderer *r, const Score *score);

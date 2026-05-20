#pragma once

#include <SDL3/SDL.h>
#include "../game/maze.h"
#include "../game/pacman.h"
#include "../game/score.h"

void draw_maze(SDL_Renderer *renderer, const Maze *maze);
void draw_pacman(SDL_Renderer *renderer, const Pacman *pacman);
void draw_hud(SDL_Renderer *renderer, const Score *score);

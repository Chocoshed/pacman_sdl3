#pragma once

#include <SDL3/SDL.h>
#include "../game/maze.h"
#include "../game/pacman.h"
#include "../game/score.h"
#include "../game/ghost.h"

void draw_maze(SDL_Renderer *renderer, const Maze *maze);
void draw_pacman(SDL_Renderer *renderer, const Pacman *pacman);
void draw_ghost(SDL_Renderer *renderer, const Ghost *ghost);
void draw_hud(SDL_Renderer *renderer, const Score *score);
void draw_fruit(SDL_Renderer *renderer, const Maze *maze, int level);
void draw_title(SDL_Renderer *renderer);
void draw_menu(SDL_Renderer *renderer, int selected_option);
void draw_pause(SDL_Renderer *renderer);
void draw_game_over(SDL_Renderer *renderer, const Score *score);

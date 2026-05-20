#pragma once

#include "../input/input.h"
#include "maze.h"

#define PACMAN_SPEED 0.15f

typedef struct {
    int       col;
    int       row;
    Direction dir_current;
    Direction dir_buffered;
    float     move_timer;
} Pacman;

void pacman_init(Pacman *pacman);
void pacman_update(Pacman *pacman, const Maze *maze, Direction input, float delta_time);

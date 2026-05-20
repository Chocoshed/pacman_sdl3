#pragma once

#include "../input/input.h"
#include "maze.h"
#include "score.h"

#define PACMAN_SPEED 0.15f

typedef struct {
    int       col;
    int       row;
    Direction dir_current;
    Direction dir_buffered;
    float     move_timer;
} Pacman;

void pacman_init(Pacman *pacman);
void pacman_reset(Pacman *pacman);
void pacman_update(Pacman *pacman, Maze *maze, Score *score, Direction input, float delta_time);

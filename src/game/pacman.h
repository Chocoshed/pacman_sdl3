#pragma once

#include "../types.h"
#include "maze.h"
#include "score.h"

#define PACMAN_SPEED 0.15f

/*
 * Struct nommée pour permettre la déclaration anticipée dans ghost.h
 * sans inclure ce header complet.
 */
typedef struct Pacman {
    int       col;
    int       row;
    Direction dir_current;
    Direction dir_buffered;
    float     move_timer;
} Pacman;

void pacman_init(Pacman *pacman);
void pacman_reset(Pacman *pacman);
void pacman_update(Pacman *pacman, Maze *maze, Score *score, Direction input, float delta_time);

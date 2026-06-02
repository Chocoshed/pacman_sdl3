#pragma once

#include "../types.h"
#include "maze.h"
#include "score.h"

#define PACMAN_SPEED        0.15f
#define PACMAN_DEATH_FRAMES 11

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
    int       anim_frame;
    float     anim_timer;
} Pacman;

void pacman_init(Pacman *pacman);
void pacman_reset(Pacman *pacman);
void pacman_update(Pacman *pacman, Maze *maze, Score *score, Direction input, float delta_time);
void pacman_update_anim(Pacman *pacman, float delta_time);

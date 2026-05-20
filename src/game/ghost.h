#pragma once

#include "maze.h"
#include "../input/input.h"

#define GHOST_COUNT 4
#define GHOST_SPEED 0.2f

typedef enum {
    GHOST_SCATTER    = 0,
    GHOST_CHASE      = 1,
    GHOST_FRIGHTENED = 2,
    GHOST_EATEN      = 3,
} GhostMode;

typedef enum {
    GHOST_BLINKY = 0,
    GHOST_PINKY  = 1,
    GHOST_INKY   = 2,
    GHOST_CLYDE  = 3,
} GhostId;

typedef struct {
    GhostId   id;
    int       col;
    int       row;
    Direction dir;
    GhostMode mode;
    float     move_timer;
} Ghost;

typedef struct {
    Ghost ghosts[GHOST_COUNT];
} Ghosts;

void ghosts_init(Ghosts *ghosts);
void ghosts_update(Ghosts *ghosts, const Maze *maze, float delta_time);

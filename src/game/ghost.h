#pragma once

#include "maze.h"
#include "pacman.h"

#define GHOST_COUNT               4
#define GHOST_SPEED               0.2f
#define GHOST_FRIGHTENED_DURATION 6.0f
#define GHOST_EATEN_DURATION      5.0f

typedef enum {
    GHOST_SCATTER    = 0,
    GHOST_CHASE      = 1,
    GHOST_FRIGHTENED = 2,
    GHOST_EATEN      = 3,
    GHOST_IN_HOUSE   = 4,  /* waiting/exiting ghost house */
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
    float     frightened_timer;
    float     eaten_timer;
} Ghost;

typedef struct {
    Ghost ghosts[GHOST_COUNT];
    int   sc_index;  /* current scatter/chase phase (0-7) */
    float sc_timer;  /* time remaining in current phase */
    bool  in_chase;  /* true = CHASE, false = SCATTER */
} Ghosts;

void ghosts_init(Ghosts *ghosts);
void ghosts_update(Ghosts *ghosts, const Maze *maze, const Pacman *pacman, float delta_time);
void ghosts_set_frightened(Ghosts *ghosts);
void ghost_set_eaten(Ghost *ghost);
void ghosts_reset(Ghosts *ghosts);

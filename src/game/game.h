#pragma once

#include "maze.h"
#include "pacman.h"
#include "ghost.h"
#include "score.h"

typedef enum {
    PHASE_PLAY,
    PHASE_DYING,
    PHASE_GAME_OVER,
    PHASE_LEVEL_CLEAR,
} GamePhase;

#define DEATH_PAUSE_DURATION 1.5f
#define LEVEL_PAUSE_DURATION 2.0f

void game_check_collisions(GamePhase *phase, Pacman *pacman, Ghosts *ghosts, Score *score);
void game_reset_positions(Pacman *pacman, Ghosts *ghosts);

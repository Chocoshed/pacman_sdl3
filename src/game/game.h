#pragma once

#include "maze.h"
#include "pacman.h"
#include "ghost.h"
#include "score.h"

typedef enum {
    PHASE_TITLE,
    PHASE_MENU,
    PHASE_PLAY,
    PHASE_PAUSE,
    PHASE_DYING,
    PHASE_LEVEL_CLEAR,
    PHASE_GAME_OVER,
} GamePhase;

typedef enum {
    MENU_NEW_GAME = 0,
    MENU_QUIT     = 1,
    MENU_OPTION_COUNT,
} MenuOption;

typedef struct {
    MenuOption selected;
} MenuState;

#define DEATH_PAUSE_DURATION 1.5f
#define LEVEL_PAUSE_DURATION 2.0f

void game_check_collisions(GamePhase *phase, Pacman *pacman, Ghosts *ghosts, Score *score);
void game_reset_positions(Pacman *pacman, Ghosts *ghosts);

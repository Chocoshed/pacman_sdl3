#pragma once

#include <stdbool.h>
#include "maze.h"
#include "../types.h"

/*
 * Déclarations anticipées — ghost.h n'inclut PAS pacman.h.
 * ISP : évite de transmettre à tous les consommateurs de ghost.h
 *       les headers de Pacman, InputState, Score, etc.
 * ghost.c inclut pacman.h directement pour accéder aux champs.
 */
typedef struct Pacman  Pacman;
typedef struct Ghost   Ghost;
typedef struct Ghosts  Ghosts;

/* ============================================================
 * CONSTANTES
 * ============================================================ */

#define GHOST_COUNT               4
#define GHOST_SPEED               0.2f
#define GHOST_FRIGHTENED_DURATION 6.0f
#define GHOST_EATEN_DURATION      5.0f

/* ============================================================
 * TYPES
 * ============================================================ */

typedef enum {
    GHOST_SCATTER    = 0,
    GHOST_CHASE      = 1,
    GHOST_FRIGHTENED = 2,
    GHOST_EATEN      = 3,
    GHOST_IN_HOUSE   = 4,  /* en attente ou sortie de la maison */
} GhostMode;

typedef enum {
    GHOST_BLINKY = 0,
    GHOST_PINKY  = 1,
    GHOST_INKY   = 2,
    GHOST_CLYDE  = 3,
} GhostId;

/*
 * O/C : chaque fantôme possède un pointeur vers sa propre fonction de cible.
 * Ajouter un nouveau fantôme = ajouter une fonction, sans modifier l'existant.
 */
typedef void (*GhostTargetFn)(const Ghost *, const Ghosts *, const Pacman *,
                               int *tcol, int *trow);

struct Ghost {
    GhostId       id;
    int           col;
    int           row;
    Direction     dir;
    GhostMode     mode;
    float         move_timer;
    float         frightened_timer;
    float         eaten_timer;
    GhostTargetFn compute_target;  /* fn d'IA spécifique à ce fantôme */
};

struct Ghosts {
    Ghost ghosts[GHOST_COUNT];
    int   sc_index;  /* phase scatter/chase courante (0-7) */
    float sc_timer;  /* temps restant dans la phase       */
    bool  in_chase;  /* true = CHASE, false = SCATTER     */
};

/* ============================================================
 * API PUBLIQUE
 * ============================================================ */

void ghosts_init(Ghosts *ghosts);
void ghosts_update(Ghosts *ghosts, const Maze *maze, const Pacman *pacman, float delta_time);
void ghosts_set_frightened(Ghosts *ghosts);
void ghost_set_eaten(Ghost *ghost);
void ghosts_reset(Ghosts *ghosts);

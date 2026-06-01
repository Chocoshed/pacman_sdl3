#pragma once

#include <SDL3/SDL.h>
#include "maze.h"
#include "pacman.h"
#include "ghost.h"
#include "score.h"
#include "../input/input.h"

/* ============================================================
 * PHASE DE JEU
 * ============================================================ */

typedef enum {
    PHASE_TITLE,       /* écran titre                     */
    PHASE_MENU,        /* menu principal                  */
    PHASE_PLAY,        /* partie en cours                 */
    PHASE_PAUSE,       /* jeu suspendu                    */
    PHASE_DYING,       /* animation mort Pac-Man          */
    PHASE_LEVEL_CLEAR, /* niveau terminé                  */
    PHASE_GAME_OVER,   /* fin de partie                   */
} GamePhase;

typedef enum {
    MENU_NEW_GAME = 0,
    MENU_QUIT     = 1,
} MenuOption;

typedef struct {
    MenuOption selected;
} MenuState;

#define DEATH_PAUSE_DURATION 1.5f  /* secondes avant respawn après mort  */
#define LEVEL_PAUSE_DURATION 2.0f  /* secondes avant le niveau suivant   */

/* ============================================================
 * ÉTAT GLOBAL DU JEU
 * ============================================================ */

typedef struct {
    Maze       maze;
    InputState input;
    Pacman     pacman;
    Score      score;
    Ghosts     ghosts;
    MenuState  menu;
    GamePhase  phase;
    float      phase_timer;
    bool       should_quit;  /* vrai = l'application doit se fermer */
} Game;

/* ============================================================
 * API PUBLIQUE
 * ============================================================ */

void game_init(Game *game);
void game_update(Game *game, float delta_time);
void game_render(const Game *game, SDL_Renderer *renderer);

#include "game.h"

void game_check_collisions(GamePhase *phase, Pacman *pacman, Ghosts *ghosts, Score *score) {
    if (*phase != PHASE_PLAY)
        return;

    for (int i = 0; i < GHOST_COUNT; i++) {
        Ghost *g = &ghosts->ghosts[i];

        if (g->col != pacman->col || g->row != pacman->row)
            continue;

        if (g->mode == GHOST_FRIGHTENED) {
            ghost_set_eaten(g);
            score_eat_ghost(score);
        } else if (g->mode == GHOST_SCATTER || g->mode == GHOST_CHASE) {
            score_lose_life(score);
            *phase = (score->lives == 0) ? PHASE_GAME_OVER : PHASE_DYING;
            return;
        }
    }
}

void game_reset_positions(Pacman *pacman, Ghosts *ghosts) {
    pacman_reset(pacman);
    ghosts_reset(ghosts);
}

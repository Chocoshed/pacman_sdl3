#include "ghost.h"
#include "pacman.h"   /* include complet nécessaire pour accéder aux champs */
#include <SDL3/SDL.h>
#include <limits.h>

/* ============================================================
 * DONNÉES : POSITIONS ET CIBLES
 * ============================================================ */

static const struct { int col; int row; } GHOST_START[GHOST_COUNT] = {
    [GHOST_BLINKY] = { 13, 14 },
    [GHOST_PINKY]  = { 13, 17 },
    [GHOST_INKY]   = { 11, 17 },
    [GHOST_CLYDE]  = { 15, 17 },
};

/* Coins cibles en mode SCATTER — un par fantôme */
static const struct { int col; int row; } SCATTER_TARGET[GHOST_COUNT] = {
    [GHOST_BLINKY] = { 25,  0 },
    [GHOST_PINKY]  = {  2,  0 },
    [GHOST_INKY]   = { 27, 35 },
    [GHOST_CLYDE]  = {  0, 35 },
};

/* Dots mangés avant autorisation de sortie de la maison */
static const int RELEASE_DOTS[GHOST_COUNT] = {
    [GHOST_BLINKY] = 0,
    [GHOST_PINKY]  = 0,
    [GHOST_INKY]   = 30,
    [GHOST_CLYDE]  = 60,
};

#define EXIT_COL    13   /* cible de sortie au-dessus de la porte */
#define EXIT_ROW    11
#define OUTSIDE_ROW 14   /* row <= OUTSIDE_ROW = fantôme sorti    */

/* ============================================================
 * DONNÉES : ALTERNANCE SCATTER / CHASE (timing niveau 1)
 * ============================================================ */

static const float SC_DURATIONS[] = {
    7.0f,   /* 0 SCATTER */
    20.0f,  /* 1 CHASE   */
    7.0f,   /* 2 SCATTER */
    20.0f,  /* 3 CHASE   */
    5.0f,   /* 4 SCATTER */
    20.0f,  /* 5 CHASE   */
    5.0f,   /* 6 SCATTER */
    1e9f,   /* 7 CHASE ∞ */
};
#define SC_PHASE_COUNT 8

/* ============================================================
 * DONNÉES : RESTRICTIONS NO-UP
 * ============================================================ */

static const struct { int col; int row; } NO_UP_TILES[] = {
    {  6, 17 }, { 21, 17 },
    {  6, 23 }, { 21, 23 },
};
#define NO_UP_COUNT 4

/* ============================================================
 * UTILITAIRES
 * ============================================================ */

static Direction dir_opposite(Direction dir) {
    switch (dir) {
        case DIR_UP:    return DIR_DOWN;
        case DIR_DOWN:  return DIR_UP;
        case DIR_LEFT:  return DIR_RIGHT;
        case DIR_RIGHT: return DIR_LEFT;
        default:        return DIR_NONE;
    }
}

static void dir_to_offset(Direction dir, int *dcol, int *drow) {
    *dcol = 0; *drow = 0;
    switch (dir) {
        case DIR_UP:    *drow = -1; break;
        case DIR_DOWN:  *drow =  1; break;
        case DIR_LEFT:  *dcol = -1; break;
        case DIR_RIGHT: *dcol =  1; break;
        default: break;
    }
}

static unsigned dist_sq(int c1, int r1, int c2, int r2) {
    int dc = c1 - c2, dr = r1 - r2;
    return (unsigned)(dc * dc + dr * dr);
}

static bool is_no_up_tile(int col, int row) {
    for (int i = 0; i < NO_UP_COUNT; i++)
        if (NO_UP_TILES[i].col == col && NO_UP_TILES[i].row == row)
            return true;
    return false;
}

/* ============================================================
 * FONCTIONS DE CIBLE PAR FANTÔME (Open/Closed)
 *
 * Chaque fantôme a sa propre implémentation de GhostTargetFn.
 * Ajouter un fantôme = ajouter une fonction ici + une entrée dans
 * TARGET_FNS. Aucune modification du code existant.
 * ============================================================ */

static void target_blinky(const Ghost *ghost, const Ghosts *ghosts,
                            const Pacman *pacman, int *tcol, int *trow) {
    (void)ghost; (void)ghosts;
    /* Poursuite directe : cible = position exacte de Pac-Man */
    *tcol = pacman->col;
    *trow = pacman->row;
}

static void target_pinky(const Ghost *ghost, const Ghosts *ghosts,
                           const Pacman *pacman, int *tcol, int *trow) {
    (void)ghost; (void)ghosts;
    /*
     * Embuscade : 4 cases devant Pac-Man.
     * Bug original reproduit : DIR_UP décale aussi de 4 vers la gauche
     * (débordement d'entier non signé dans le code 1980).
     */
    int dcol = 0, drow = 0;
    dir_to_offset(pacman->dir_current, &dcol, &drow);
    *tcol = pacman->col + dcol * 4;
    *trow = pacman->row + drow * 4;
    if (pacman->dir_current == DIR_UP)
        *tcol -= 4;
}

static void target_inky(const Ghost *ghost, const Ghosts *ghosts,
                          const Pacman *pacman, int *tcol, int *trow) {
    /*
     * Flanc : pivot = 2 cases devant Pac-Man (avec bug UP reproduit),
     * cible = symétrie du pivot par rapport à Blinky.
     */
    int dcol = 0, drow = 0;
    dir_to_offset(pacman->dir_current, &dcol, &drow);
    int pivot_col = pacman->col + dcol * 2;
    int pivot_row = pacman->row + drow * 2;
    if (pacman->dir_current == DIR_UP)
        pivot_col -= 2;
    const Ghost *blinky = &ghosts->ghosts[GHOST_BLINKY];
    *tcol = pivot_col + (pivot_col - blinky->col);
    *trow = pivot_row + (pivot_row - blinky->row);
    (void)ghost;
}

static void target_clyde(const Ghost *ghost, const Ghosts *ghosts,
                           const Pacman *pacman, int *tcol, int *trow) {
    (void)ghosts;
    /*
     * Timide : pourchasse si distance > 8 cases,
     * sinon fuit vers son coin bas-gauche.
     */
    if (dist_sq(ghost->col, ghost->row, pacman->col, pacman->row) > 64) {
        *tcol = pacman->col;
        *trow = pacman->row;
    } else {
        *tcol = SCATTER_TARGET[GHOST_CLYDE].col;
        *trow = SCATTER_TARGET[GHOST_CLYDE].row;
    }
}

/* Table de dispatch : lie chaque GhostId à sa fonction de cible */
static const GhostTargetFn TARGET_FNS[GHOST_COUNT] = {
    [GHOST_BLINKY] = target_blinky,
    [GHOST_PINKY]  = target_pinky,
    [GHOST_INKY]   = target_inky,
    [GHOST_CLYDE]  = target_clyde,
};

/* ============================================================
 * RÉINITIALISATION
 * ============================================================ */

static void ghost_reset(Ghost *g) {
    g->col              = GHOST_START[g->id].col;
    g->row              = GHOST_START[g->id].row;
    g->dir              = DIR_LEFT;
    g->mode             = (g->id == GHOST_BLINKY) ? GHOST_SCATTER : GHOST_IN_HOUSE;
    g->move_timer       = 0.0f;
    g->frightened_timer = 0.0f;
    g->eaten_timer      = 0.0f;
    g->compute_target   = TARGET_FNS[g->id];
}

/* ============================================================
 * MOUVEMENT
 *
 * DIP : utilise maze_is_passable et maze_tunnel_col au lieu
 *       d'accéder directement à maze->cells.
 * ============================================================ */

static void move_toward(Ghost *ghost, const Maze *maze, int tcol, int trow) {
    static const Direction PRIORITY[] = { DIR_UP, DIR_LEFT, DIR_DOWN, DIR_RIGHT };

    bool      door_ok   = (ghost->mode == GHOST_EATEN || ghost->mode == GHOST_IN_HOUSE);
    Direction forbidden = dir_opposite(ghost->dir);
    Direction chosen    = DIR_NONE;
    unsigned  best_dist = UINT_MAX;

    for (int i = 0; i < 4; i++) {
        Direction d = PRIORITY[i];
        if (d == forbidden)
            continue;
        if (d == DIR_UP && is_no_up_tile(ghost->col, ghost->row))
            continue;

        int dcol, drow;
        dir_to_offset(d, &dcol, &drow);
        int nc = ghost->col + dcol;
        int nr = ghost->row + drow;
        if (!maze_is_passable(maze, nc, nr, door_ok))
            continue;

        unsigned dist = dist_sq(maze_tunnel_col(nc), nr, tcol, trow);
        if (dist < best_dist) {
            best_dist = dist;
            chosen    = d;
        }
    }

    if (chosen == DIR_NONE)
        return;

    int dcol, drow;
    dir_to_offset(chosen, &dcol, &drow);
    ghost->dir  = chosen;
    ghost->col  = maze_tunnel_col(ghost->col + dcol);
    ghost->row += drow;
}

static void move_random(Ghost *ghost, const Maze *maze) {
    static const Direction ALL_DIRS[] = { DIR_UP, DIR_DOWN, DIR_LEFT, DIR_RIGHT };

    bool      door_ok   = (ghost->mode == GHOST_EATEN || ghost->mode == GHOST_IN_HOUSE);
    Direction forbidden = dir_opposite(ghost->dir);
    Direction valid[4];
    int       count = 0;

    for (int i = 0; i < 4; i++) {
        Direction d = ALL_DIRS[i];
        if (d == forbidden)
            continue;
        int dcol, drow;
        dir_to_offset(d, &dcol, &drow);
        if (maze_is_passable(maze, ghost->col + dcol, ghost->row + drow, door_ok))
            valid[count++] = d;
    }

    if (count == 0)
        return;

    Direction chosen = valid[SDL_rand(count)];
    int dcol, drow;
    dir_to_offset(chosen, &dcol, &drow);
    ghost->dir  = chosen;
    ghost->col  = maze_tunnel_col(ghost->col + dcol);
    ghost->row += drow;
}

/* ============================================================
 * API PUBLIQUE
 * ============================================================ */

void ghosts_init(Ghosts *ghosts) {
    for (int i = 0; i < GHOST_COUNT; i++) {
        Ghost *g = &ghosts->ghosts[i];
        g->id    = (GhostId)i;
        ghost_reset(g);
        g->move_timer = (float)i * (GHOST_SPEED / GHOST_COUNT);
    }
    ghosts->sc_index = 0;
    ghosts->sc_timer = SC_DURATIONS[0];
    ghosts->in_chase = false;
}

void ghosts_update(Ghosts *ghosts, const Maze *maze, const Pacman *pacman, float delta_time) {

    /* -- Phase scatter/chase -- */
    ghosts->sc_timer -= delta_time;
    if (ghosts->sc_timer <= 0.0f && ghosts->sc_index < SC_PHASE_COUNT - 1) {
        ghosts->sc_index++;
        ghosts->sc_timer = SC_DURATIONS[ghosts->sc_index];
        bool new_chase   = (ghosts->sc_index % 2 == 1);
        if (new_chase != ghosts->in_chase) {
            ghosts->in_chase   = new_chase;
            GhostMode new_mode = new_chase ? GHOST_CHASE : GHOST_SCATTER;
            for (int i = 0; i < GHOST_COUNT; i++) {
                Ghost *g = &ghosts->ghosts[i];
                if (g->mode == GHOST_SCATTER || g->mode == GHOST_CHASE) {
                    g->mode = new_mode;
                    g->dir  = dir_opposite(g->dir);
                }
            }
        }
    }

    /* -- Mise à jour individuelle -- */
    for (int i = 0; i < GHOST_COUNT; i++) {
        Ghost *g = &ghosts->ghosts[i];

        if (g->mode == GHOST_FRIGHTENED) {
            g->frightened_timer -= delta_time;
            if (g->frightened_timer <= 0.0f)
                g->mode = ghosts->in_chase ? GHOST_CHASE : GHOST_SCATTER;
        }
        if (g->mode == GHOST_EATEN) {
            g->eaten_timer -= delta_time;
            if (g->eaten_timer <= 0.0f)
                ghost_reset(g);
        }

        g->move_timer += delta_time;
        if (g->move_timer < GHOST_SPEED)
            continue;
        g->move_timer = 0.0f;

        switch (g->mode) {

            case GHOST_IN_HOUSE:
                if (maze->dots_eaten >= RELEASE_DOTS[g->id]) {
                    move_toward(g, maze, EXIT_COL, EXIT_ROW);
                    if (g->row <= OUTSIDE_ROW)
                        g->mode = ghosts->in_chase ? GHOST_CHASE : GHOST_SCATTER;
                }
                break;

            case GHOST_FRIGHTENED:
                move_random(g, maze);
                break;

            case GHOST_EATEN:
                move_toward(g, maze,
                    GHOST_START[GHOST_PINKY].col, GHOST_START[GHOST_PINKY].row);
                break;

            case GHOST_SCATTER:
                move_toward(g, maze, SCATTER_TARGET[g->id].col, SCATTER_TARGET[g->id].row);
                break;

            case GHOST_CHASE: {
                int tcol, trow;
                g->compute_target(g, ghosts, pacman, &tcol, &trow);
                move_toward(g, maze, tcol, trow);
                break;
            }
        }
    }
}

void ghosts_set_frightened(Ghosts *ghosts) {
    for (int i = 0; i < GHOST_COUNT; i++) {
        Ghost *g = &ghosts->ghosts[i];
        if (g->mode != GHOST_EATEN && g->mode != GHOST_IN_HOUSE) {
            g->mode             = GHOST_FRIGHTENED;
            g->frightened_timer = GHOST_FRIGHTENED_DURATION;
        }
    }
}

void ghost_set_eaten(Ghost *ghost) {
    ghost->mode        = GHOST_EATEN;
    ghost->eaten_timer = GHOST_EATEN_DURATION;
}

void ghosts_reset(Ghosts *ghosts) {
    for (int i = 0; i < GHOST_COUNT; i++)
        ghost_reset(&ghosts->ghosts[i]);
    ghosts->sc_index = 0;
    ghosts->sc_timer = SC_DURATIONS[0];
    ghosts->in_chase = false;
}

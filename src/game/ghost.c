#include "ghost.h"
#include <SDL3/SDL.h>
#include <limits.h>

/* ---- Scatter corner targets (original Pac-Man corners) ---- */
static const struct { int col; int row; } SCATTER_TARGET[GHOST_COUNT] = {
    [GHOST_BLINKY] = { 25,  0 },
    [GHOST_PINKY]  = {  2,  0 },
    [GHOST_INKY]   = { 27, 35 },
    [GHOST_CLYDE]  = {  0, 35 },
};

/* ---- Starting positions ---- */
static const struct { int col; int row; } GHOST_START[GHOST_COUNT] = {
    [GHOST_BLINKY] = { 13, 14 },
    [GHOST_PINKY]  = { 13, 17 },
    [GHOST_INKY]   = { 11, 17 },
    [GHOST_CLYDE]  = { 15, 17 },
};

/*
 * Dots eaten before each ghost is allowed to exit the house.
 * Blinky starts outside; Pinky exits immediately; Inky at 30; Clyde at 60.
 */
static const int RELEASE_DOTS[GHOST_COUNT] = {
    [GHOST_BLINKY] = 0,   /* never in house */
    [GHOST_PINKY]  = 0,
    [GHOST_INKY]   = 30,
    [GHOST_CLYDE]  = 60,
};

/* Target tile for house exit — above the door, clearly outside */
#define EXIT_COL 13
#define EXIT_ROW 11
/* Row at which a ghost is considered to have left the house */
#define OUTSIDE_ROW 14

/*
 * Scatter/Chase phase durations (seconds) — Level 1 timing from original.
 * Even indices = SCATTER, odd indices = CHASE. Last phase = CHASE forever.
 */
static const float SC_DURATIONS[] = {
    7.0f,   /* 0: SCATTER */
    20.0f,  /* 1: CHASE   */
    7.0f,   /* 2: SCATTER */
    20.0f,  /* 3: CHASE   */
    5.0f,   /* 4: SCATTER */
    20.0f,  /* 5: CHASE   */
    5.0f,   /* 6: SCATTER */
    1e9f,   /* 7: CHASE ∞ */
};
#define SC_PHASE_COUNT 8

/*
 * No-up-turn tiles: original Pac-Man forbids ghosts from turning upward at
 * these T-junctions to prevent shortcut exploitation.
 */
static const struct { int col; int row; } NO_UP_TILES[] = {
    {  6, 17 }, { 21, 17 },
    {  6, 23 }, { 21, 23 },
};
#define NO_UP_COUNT 4

/* ---- Helpers ---- */

static Direction dir_opposite(Direction dir) {
    switch (dir) {
        case DIR_UP:    return DIR_DOWN;
        case DIR_DOWN:  return DIR_UP;
        case DIR_LEFT:  return DIR_RIGHT;
        case DIR_RIGHT: return DIR_LEFT;
        default:        return DIR_NONE;
    }
}

static void dir_offset(Direction dir, int *dcol, int *drow) {
    *dcol = 0; *drow = 0;
    switch (dir) {
        case DIR_UP:    *drow = -1; break;
        case DIR_DOWN:  *drow =  1; break;
        case DIR_LEFT:  *dcol = -1; break;
        case DIR_RIGHT: *dcol =  1; break;
        default: break;
    }
}

/*
 * Horizontal tunnel: col wraps around. Ghosts in GHOST_IN_HOUSE or GHOST_EATEN
 * can pass through the ghost door.
 */
static bool cell_passable(const Maze *maze, int col, int row, GhostMode mode) {
    /* Tunnel wraparound */
    if (col < 0)            col = MAZE_COLS - 1;
    else if (col >= MAZE_COLS) col = 0;

    if (row < 0 || row >= MAZE_ROWS)
        return false;

    CellType cell = (CellType)maze->cells[row][col];
    if (cell == CELL_WALL)
        return false;
    if (cell == CELL_GHOST_DOOR &&
        mode != GHOST_EATEN && mode != GHOST_IN_HOUSE)
        return false;
    return true;
}

static bool is_no_up_tile(int col, int row) {
    for (int i = 0; i < NO_UP_COUNT; i++)
        if (NO_UP_TILES[i].col == col && NO_UP_TILES[i].row == row)
            return true;
    return false;
}

static unsigned dist_sq(int c1, int r1, int c2, int r2) {
    int dc = c1 - c2, dr = r1 - r2;
    return (unsigned)(dc * dc + dr * dr);
}

static void tunnel_wrap(Ghost *ghost) {
    if (ghost->col < 0)              ghost->col = MAZE_COLS - 1;
    else if (ghost->col >= MAZE_COLS) ghost->col = 0;
}

static void ghost_reset_single(Ghost *g) {
    g->col              = GHOST_START[g->id].col;
    g->row              = GHOST_START[g->id].row;
    g->dir              = DIR_LEFT;
    g->mode             = (g->id == GHOST_BLINKY) ? GHOST_SCATTER : GHOST_IN_HOUSE;
    g->move_timer       = 0.0f;
    g->frightened_timer = 0.0f;
    g->eaten_timer      = 0.0f;
}

/* ---- Movement ---- */

/*
 * Target-based movement: choose direction minimising Euclidean² distance to
 * target. Priority when tied: UP > LEFT > DOWN > RIGHT (original Pac-Man).
 * Ghosts cannot reverse and cannot go UP on no-up tiles.
 */
static void ghost_move_toward(Ghost *ghost, const Maze *maze, int tcol, int trow) {
    static const Direction PRIORITY[] = { DIR_UP, DIR_LEFT, DIR_DOWN, DIR_RIGHT };

    Direction forbidden = dir_opposite(ghost->dir);
    Direction chosen    = DIR_NONE;
    unsigned  best      = UINT_MAX;

    for (int i = 0; i < 4; i++) {
        Direction d = PRIORITY[i];
        if (d == forbidden)
            continue;
        if (d == DIR_UP && is_no_up_tile(ghost->col, ghost->row))
            continue;
        int dcol, drow;
        dir_offset(d, &dcol, &drow);
        int nc = ghost->col + dcol;
        int nr = ghost->row + drow;
        if (!cell_passable(maze, nc, nr, ghost->mode))
            continue;

        /* Use wrapped col for distance so tunnel entry gets correct distance */
        int wc = nc;
        if (wc < 0)           wc = MAZE_COLS - 1;
        else if (wc >= MAZE_COLS) wc = 0;

        unsigned dist = dist_sq(wc, nr, tcol, trow);
        if (dist < best) {
            best   = dist;
            chosen = d;
        }
    }

    if (chosen == DIR_NONE)
        return;

    int dcol, drow;
    dir_offset(chosen, &dcol, &drow);
    ghost->dir  = chosen;
    ghost->col += dcol;
    ghost->row += drow;
    tunnel_wrap(ghost);
}

/* FRIGHTENED: random valid direction (cannot reverse) */
static void ghost_move_random(Ghost *ghost, const Maze *maze) {
    static const Direction ALL_DIRS[] = { DIR_UP, DIR_DOWN, DIR_LEFT, DIR_RIGHT };
    Direction forbidden = dir_opposite(ghost->dir);
    Direction valid[4];
    int       count = 0;

    for (int i = 0; i < 4; i++) {
        Direction d = ALL_DIRS[i];
        if (d == forbidden)
            continue;
        int dcol, drow;
        dir_offset(d, &dcol, &drow);
        if (cell_passable(maze, ghost->col + dcol, ghost->row + drow, ghost->mode))
            valid[count++] = d;
    }

    if (count == 0)
        return;

    Direction chosen = valid[SDL_rand(count)];
    int dcol, drow;
    dir_offset(chosen, &dcol, &drow);
    ghost->dir  = chosen;
    ghost->col += dcol;
    ghost->row += drow;
    tunnel_wrap(ghost);
}

/* ---- Chase target computation (per original 1980 logic) ---- */

static void compute_chase_target(const Ghost *ghost, const Ghosts *ghosts,
                                  const Pacman *pacman, int *tcol, int *trow) {
    switch (ghost->id) {

        case GHOST_BLINKY:
            /* Direct pursuit of Pac-Man */
            *tcol = pacman->col;
            *trow = pacman->row;
            break;

        case GHOST_PINKY: {
            /* 4 tiles ahead of Pac-Man.
             * Replicates original overflow bug: DIR_UP also offsets 4 left. */
            int dcol = 0, drow = 0;
            dir_offset(pacman->dir_current, &dcol, &drow);
            *tcol = pacman->col + dcol * 4;
            *trow = pacman->row + drow * 4;
            if (pacman->dir_current == DIR_UP)
                *tcol -= 4;
            break;
        }

        case GHOST_INKY: {
            /* Pivot = 2 tiles ahead of Pac-Man (with UP overflow bug);
             * target = Blinky + 2 * (pivot - Blinky). */
            int dcol = 0, drow = 0;
            dir_offset(pacman->dir_current, &dcol, &drow);
            int pivot_col = pacman->col + dcol * 2;
            int pivot_row = pacman->row + drow * 2;
            if (pacman->dir_current == DIR_UP)
                pivot_col -= 2;
            const Ghost *blinky = &ghosts->ghosts[GHOST_BLINKY];
            *tcol = pivot_col + (pivot_col - blinky->col);
            *trow = pivot_row + (pivot_row - blinky->row);
            break;
        }

        case GHOST_CLYDE:
            /* Chases Pac-Man if > 8 tiles away, else retreats to scatter corner */
            if (dist_sq(ghost->col, ghost->row, pacman->col, pacman->row) > 64) {
                *tcol = pacman->col;
                *trow = pacman->row;
            } else {
                *tcol = SCATTER_TARGET[GHOST_CLYDE].col;
                *trow = SCATTER_TARGET[GHOST_CLYDE].row;
            }
            break;
    }
}

/* ---- Public API ---- */

void ghosts_init(Ghosts *ghosts) {
    for (int i = 0; i < GHOST_COUNT; i++) {
        Ghost *g = &ghosts->ghosts[i];
        g->id    = (GhostId)i;
        ghost_reset_single(g);
        g->move_timer = (float)i * (GHOST_SPEED / GHOST_COUNT);
    }
    ghosts->sc_index = 0;
    ghosts->sc_timer = SC_DURATIONS[0];
    ghosts->in_chase = false;
}

void ghosts_update(Ghosts *ghosts, const Maze *maze, const Pacman *pacman, float delta_time) {
    /* Advance scatter/chase phase timer */
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
                    g->dir  = dir_opposite(g->dir); /* mandatory reversal on switch */
                }
            }
        }
    }

    for (int i = 0; i < GHOST_COUNT; i++) {
        Ghost *g = &ghosts->ghosts[i];

        /* Timers */
        if (g->mode == GHOST_FRIGHTENED) {
            g->frightened_timer -= delta_time;
            if (g->frightened_timer <= 0.0f)
                g->mode = ghosts->in_chase ? GHOST_CHASE : GHOST_SCATTER;
        }
        if (g->mode == GHOST_EATEN) {
            g->eaten_timer -= delta_time;
            if (g->eaten_timer <= 0.0f)
                ghost_reset_single(g);
        }

        /* Movement tick */
        g->move_timer += delta_time;
        if (g->move_timer < GHOST_SPEED)
            continue;
        g->move_timer = 0.0f;

        switch (g->mode) {
            case GHOST_IN_HOUSE:
                if (maze->dots_eaten >= RELEASE_DOTS[g->id]) {
                    ghost_move_toward(g, maze, EXIT_COL, EXIT_ROW);
                    if (g->row <= OUTSIDE_ROW)
                        g->mode = ghosts->in_chase ? GHOST_CHASE : GHOST_SCATTER;
                }
                break;

            case GHOST_FRIGHTENED:
                ghost_move_random(g, maze);
                break;

            case GHOST_EATEN:
                ghost_move_toward(g, maze, GHOST_START[GHOST_PINKY].col,
                                            GHOST_START[GHOST_PINKY].row);
                break;

            case GHOST_SCATTER:
                ghost_move_toward(g, maze,
                    SCATTER_TARGET[g->id].col, SCATTER_TARGET[g->id].row);
                break;

            case GHOST_CHASE: {
                int tcol, trow;
                compute_chase_target(g, ghosts, pacman, &tcol, &trow);
                ghost_move_toward(g, maze, tcol, trow);
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
        ghost_reset_single(&ghosts->ghosts[i]);
    ghosts->sc_index = 0;
    ghosts->sc_timer = SC_DURATIONS[0];
    ghosts->in_chase = false;
}

#include "ghost.h"
#include <SDL3/SDL.h>

static const struct { int col; int row; } GHOST_START[GHOST_COUNT] = {
    [GHOST_BLINKY] = { 13, 14 },
    [GHOST_PINKY]  = { 11, 17 },
    [GHOST_INKY]   = { 13, 17 },
    [GHOST_CLYDE]  = { 15, 17 },
};

static Direction opposite(Direction dir) {
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

static bool cell_passable(const Maze *maze, int col, int row, GhostMode mode) {
    if (col < 0 || col >= MAZE_COLS || row < 0 || row >= MAZE_ROWS)
        return false;
    CellType cell = (CellType)maze->cells[row][col];
    if (cell == CELL_WALL)
        return false;
    if (cell == CELL_GHOST_DOOR && mode != GHOST_EATEN)
        return false;
    return true;
}

static void ghost_move(Ghost *ghost, const Maze *maze) {
    static const Direction ALL_DIRS[] = { DIR_UP, DIR_DOWN, DIR_LEFT, DIR_RIGHT };

    Direction valid[4];
    int       valid_count = 0;
    Direction forbidden   = opposite(ghost->dir);

    for (int i = 0; i < 4; i++) {
        Direction d = ALL_DIRS[i];
        if (d == forbidden)
            continue;
        int dcol, drow;
        dir_offset(d, &dcol, &drow);
        if (cell_passable(maze, ghost->col + dcol, ghost->row + drow, ghost->mode))
            valid[valid_count++] = d;
    }

    if (valid_count == 0)
        return;

    Direction chosen = valid[SDL_rand(valid_count)];
    int dcol, drow;
    dir_offset(chosen, &dcol, &drow);
    ghost->dir  = chosen;
    ghost->col += dcol;
    ghost->row += drow;
}

void ghosts_init(Ghosts *ghosts) {
    for (int i = 0; i < GHOST_COUNT; i++) {
        Ghost *g    = &ghosts->ghosts[i];
        g->id         = (GhostId)i;
        g->col        = GHOST_START[i].col;
        g->row        = GHOST_START[i].row;
        g->dir        = DIR_LEFT;
        g->mode       = GHOST_SCATTER;
        g->move_timer = (float)i * (GHOST_SPEED / GHOST_COUNT);
    }
}

void ghosts_update(Ghosts *ghosts, const Maze *maze, float delta_time) {
    for (int i = 0; i < GHOST_COUNT; i++) {
        Ghost *g = &ghosts->ghosts[i];
        g->move_timer += delta_time;
        if (g->move_timer >= GHOST_SPEED) {
            g->move_timer = 0.0f;
            ghost_move(g, maze);
        }
    }
}

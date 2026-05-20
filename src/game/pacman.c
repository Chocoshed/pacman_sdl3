#include "pacman.h"

static void direction_offset(Direction dir, int *dcol, int *drow) {
    *dcol = 0;
    *drow = 0;
    switch (dir) {
        case DIR_UP:    *drow = -1; break;
        case DIR_DOWN:  *drow =  1; break;
        case DIR_LEFT:  *dcol = -1; break;
        case DIR_RIGHT: *dcol =  1; break;
        default: break;
    }
}

static bool can_move(const Maze *maze, int col, int row, Direction dir) {
    int dcol, drow;
    direction_offset(dir, &dcol, &drow);
    int next_col = col + dcol;
    int next_row = row + drow;
    /* tunnel : on laisse sortir par les bords */
    if (next_col < 0 || next_col >= MAZE_COLS)
        return true;
    return !maze_is_wall(maze, next_col, next_row);
}

void pacman_init(Pacman *pacman) {
    pacman->col          = 14;
    pacman->row          = 26;
    pacman->dir_current  = DIR_LEFT;
    pacman->dir_buffered = DIR_LEFT;
    pacman->move_timer   = 0.0f;
}

void pacman_update(Pacman *pacman, Maze *maze, Score *score, Direction input, float delta_time) {
    if (input != DIR_NONE)
        pacman->dir_buffered = input;

    pacman->move_timer += delta_time;
    if (pacman->move_timer < PACMAN_SPEED)
        return;
    pacman->move_timer = 0.0f;

    /* tenter la direction bufferisée en premier */
    if (pacman->dir_buffered != DIR_NONE &&
        can_move(maze, pacman->col, pacman->row, pacman->dir_buffered)) {
        pacman->dir_current = pacman->dir_buffered;
    }

    /* se déplacer dans la direction courante si possible */
    if (pacman->dir_current != DIR_NONE &&
        can_move(maze, pacman->col, pacman->row, pacman->dir_current)) {
        int dcol, drow;
        direction_offset(pacman->dir_current, &dcol, &drow);
        pacman->col += dcol;
        pacman->row += drow;

        /* tunnel gauche / droite */
        if (pacman->col < 0)
            pacman->col = MAZE_COLS - 1;
        else if (pacman->col >= MAZE_COLS)
            pacman->col = 0;

        CellType eaten = maze_eat_dot(maze, pacman->col, pacman->row);
        if (eaten == CELL_DOT) {
            score_add_points(score, 10);
        } else if (eaten == CELL_POWER_PELLET) {
            score_add_points(score, 50);
            score->power_active = true;
        }
    }
}

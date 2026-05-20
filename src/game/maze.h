#pragma once

#include <stdint.h>
#include <stdbool.h>

#define MAZE_COLS 28
#define MAZE_ROWS 36
#define CELL_SIZE 16

typedef enum {
    CELL_EMPTY        = 0,
    CELL_WALL         = 1,
    CELL_DOT          = 2,
    CELL_POWER_PELLET = 3,
    CELL_FRUIT_SPAWN  = 4,
    CELL_GHOST_DOOR   = 5,
} CellType;

typedef struct {
    uint8_t cells[MAZE_ROWS][MAZE_COLS];
    int     dots_total;
    int     dots_eaten;
} Maze;

void maze_init(Maze *maze);
bool maze_is_wall(const Maze *maze, int col, int row);
bool maze_eat_dot(Maze *maze, int col, int row);
int  maze_dots_remaining(const Maze *maze);

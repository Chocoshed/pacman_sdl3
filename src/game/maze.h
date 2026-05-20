#pragma once

#include <stdint.h>
#include <stdbool.h>

#define MAZE_COLS         28
#define MAZE_ROWS         36
#define CELL_SIZE         16
#define FRUIT_COL         13
#define FRUIT_ROW         20
#define FRUIT_DURATION    10.0f
#define FRUIT_THRESHOLD1  70
#define FRUIT_THRESHOLD2  170

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
    bool    fruit_active;
    bool    fruit_spawned_once;
    bool    fruit_spawned_twice;
    float   fruit_timer;
} Maze;

void     maze_init(Maze *maze);
bool     maze_is_wall(const Maze *maze, int col, int row);
CellType maze_eat_dot(Maze *maze, int col, int row);
int      maze_dots_remaining(const Maze *maze);
void     maze_update_fruit(Maze *maze, float delta_time);
bool     maze_try_eat_fruit(Maze *maze, int col, int row);

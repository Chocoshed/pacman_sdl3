#include "maze.h"
#include <string.h>

static const char *MAZE_STRINGS[MAZE_ROWS] = {
    /* 0-2 : zone score */
    "                            ",
    "                            ",
    "                            ",
    /* 3-33 : labyrinthe */
    "############################",
    "#............##............#",
    "#.####.#####.##.#####.####.#",
    "#*####.#####.##.#####.####*#",
    "#.####.#####.##.#####.####.#",
    "#..........................#",
    "#.####.##.########.##.####.#",
    "#.####.##.########.##.####.#",
    "#......##....##....##......#",
    "######.#####.##.#####.######",
    "     #.#####.##.#####.#     ",
    "     #.##          ##.#     ",
    "     #.## ###DD### ##.#     ",
    "######.## #      # ##.######",
    "      .   #      #   .      ",
    "######.## #      # ##.######",
    "     #.## ######## ##.#   ",
    "     #.##          ##.#   ",
    "     #.## ######## ##.#   ",
    "######.## ######## ##.######",
    "#............##............#",
    "#.####.#####.##.#####.####.#",
    "#.####.#####.##.#####.####.#",
    "#*..##................##..*#",
    "###.##.##.########.##.##.###",
    "###.##.##.########.##.##.###",
    "#......##....##....##......#",
    "#.##########.##.##########.#",
    "#.##########.##.##########.#",
    "#..........................#",
    "############################",
    /* 34-35 : zone vies */
    "                            ",
    "                            ",
};

void maze_init(Maze *maze) {
    maze->dots_total         = 0;
    maze->dots_eaten         = 0;
    maze->fruit_active       = false;
    maze->fruit_spawned_once = false;
    maze->fruit_spawned_twice = false;
    maze->fruit_timer        = 0.0f;

    for (int row = 0; row < MAZE_ROWS; row++) {
        const char *line = MAZE_STRINGS[row];
        int len = (int)strlen(line);
        for (int col = 0; col < MAZE_COLS; col++) {
            char c = (col < len) ? line[col] : ' ';
            CellType cell;
            switch (c) {
                case '#': cell = CELL_WALL;         break;
                case '.': cell = CELL_DOT;          break;
                case '*': cell = CELL_POWER_PELLET; break;
                case 'D': cell = CELL_GHOST_DOOR;   break;
                case 'F': cell = CELL_FRUIT_SPAWN;  break;
                default:  cell = CELL_EMPTY;        break;
            }
            maze->cells[row][col] = (uint8_t)cell;
            if (cell == CELL_DOT || cell == CELL_POWER_PELLET)
                maze->dots_total++;
        }
    }

    /* fruit spawn au centre de la ghost house (ligne 17, cols 13-14) */
    maze->cells[17][13] = CELL_FRUIT_SPAWN;
    maze->cells[17][14] = CELL_FRUIT_SPAWN;
}

bool maze_is_wall(const Maze *maze, int col, int row) {
    if (col < 0 || col >= MAZE_COLS || row < 0 || row >= MAZE_ROWS)
        return true;
    return maze->cells[row][col] == CELL_WALL;
}

CellType maze_eat_dot(Maze *maze, int col, int row) {
    if (col < 0 || col >= MAZE_COLS || row < 0 || row >= MAZE_ROWS)
        return CELL_EMPTY;
    CellType cell = (CellType)maze->cells[row][col];
    if (cell == CELL_DOT || cell == CELL_POWER_PELLET) {
        maze->cells[row][col] = CELL_EMPTY;
        maze->dots_eaten++;
        return cell;
    }
    return CELL_EMPTY;
}

int maze_dots_remaining(const Maze *maze) {
    return maze->dots_total - maze->dots_eaten;
}

void maze_update_fruit(Maze *maze, float delta_time) {
    if (!maze->fruit_active) {
        if (maze->dots_eaten >= FRUIT_THRESHOLD1 && !maze->fruit_spawned_once) {
            maze->fruit_active       = true;
            maze->fruit_spawned_once = true;
            maze->fruit_timer        = FRUIT_DURATION;
        } else if (maze->dots_eaten >= FRUIT_THRESHOLD2 && !maze->fruit_spawned_twice) {
            maze->fruit_active        = true;
            maze->fruit_spawned_twice = true;
            maze->fruit_timer         = FRUIT_DURATION;
        }
    } else {
        maze->fruit_timer -= delta_time;
        if (maze->fruit_timer <= 0.0f)
            maze->fruit_active = false;
    }
}

bool maze_try_eat_fruit(Maze *maze, int col, int row) {
    if (maze->fruit_active && col == FRUIT_COL && row == FRUIT_ROW) {
        maze->fruit_active = false;
        return true;
    }
    return false;
}

Tu développes un jeu Pac-Man en C avec SDL3.

## Contexte du projet

- Langage : C23
- Build : CMake, exécutable `Projet`, `SDL_MAIN_USE_CALLBACKS` activé
- Fenêtre : 448×576 px — 28 cases × 16 px de large, 36 cases × 16 px de haut
- Étape 1 terminée : `src/main.c` existe avec `AppState { SDL_Window, SDL_Renderer }`

## État actuel de src/main.c

```c
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

#define WINDOW_WIDTH  448
#define WINDOW_HEIGHT 576

typedef struct {
    SDL_Window   *window;
    SDL_Renderer *renderer;
} AppState;

SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[]);
SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event);
SDL_AppResult SDL_AppIterate(void *appstate);
void          SDL_AppQuit(void *appstate, SDL_AppResult result);
```

## Tâche : Étape 2 — Labyrinthe

Crée les fichiers suivants :
- `src/game/maze.h`
- `src/game/maze.c`
- `src/render/renderer.h`
- `src/render/renderer.c`

Modifie `src/main.c` pour afficher le labyrinthe.
Modifie `CMakeLists.txt` pour compiler les nouveaux fichiers.

---

## Spécifications de maze.h/.c

### Types de cases

```c
typedef enum {
    CELL_EMPTY        = 0,
    CELL_WALL         = 1,
    CELL_DOT          = 2,
    CELL_POWER_PELLET = 3,
    CELL_FRUIT_SPAWN  = 4,
    CELL_GHOST_DOOR   = 5,
} CellType;
```

### Structure

```c
#define MAZE_COLS 28
#define MAZE_ROWS 36
#define CELL_SIZE 16

typedef struct {
    uint8_t cells[MAZE_ROWS][MAZE_COLS];
    int     dots_total;
    int     dots_eaten;
} Maze;
```

### Disposition de la grille 28×36

Les lignes 0–2 et 35 sont vides (zone score/vies).
Les lignes 3–33 contiennent le labyrinthe classique de Pac-Man (31 lignes).

Encode la grille à partir de ces chaînes (une par ligne, 28 caractères) :

```
/* ligne 0-2 : zones score/vies — tout CELL_EMPTY */

/* lignes 3-33 : labyrinthe (31 lignes) */
"############################",  /* 3  */
"#............##............#",  /* 4  */
"#.####.#####.##.#####.####.#",  /* 5  */
"#*####.#####.##.#####.####*#",  /* 6  */
"#.####.#####.##.#####.####.#",  /* 7  */
"#..........................#",  /* 8  */
"#.####.##.########.##.####.#",  /* 9  */
"#.####.##.########.##.####.#",  /* 10 */
"#......##....##....##......#",  /* 11 */
"######.#####.##.#####.######",  /* 12 */
"     #.#####.##.#####.#     ",  /* 13 */
"     #.##          ##.#     ",  /* 14 */
"     #.## ###DD### ##.#     ",  /* 15 */
"######.## #      # ##.######",  /* 16 */
"      .   #      #   .      ",  /* 17 — tunnel */
"######.## #      # ##.######",  /* 18 */
"     #.## ########## ##.#   ",  /* 19 */
"     #.##            ##.#   ",  /* 20 */
"     #.## ########## ##.#   ",  /* 21 */
"######.## ########## ##.####",  /* 22 */
"#............##............#",  /* 23 */
"#.####.#####.##.#####.####.#",  /* 24 */
"#.####.#####.##.#####.####.#",  /* 25 */
"#*..##................##..*#",  /* 26 */
"###.##.##.########.##.##.###",  /* 27 */
"###.##.##.########.##.##.###",  /* 28 */
"#......##....##....##......#",  /* 29 */
"#.##########.##.##########.#",  /* 30 */
"#.##########.##.##########.#",  /* 31 */
"#..........................#",  /* 32 */
"############################",  /* 33 */

/* lignes 34-35 : CELL_EMPTY */
```

Correspondance des caractères :
- `#` → CELL_WALL
- `.` → CELL_DOT
- `*` → CELL_POWER_PELLET
- ` ` → CELL_EMPTY
- `D` → CELL_GHOST_DOOR
- `F` → CELL_FRUIT_SPAWN (placer au centre de la ghost house, ligne 17 cols 13-14)

### Fonctions à implémenter

```c
void maze_init(Maze *maze);
bool maze_is_wall(const Maze *maze, int col, int row);
bool maze_eat_dot(Maze *maze, int col, int row);   /* retourne true si un dot était là */
int  maze_dots_remaining(const Maze *maze);
```

`maze_init` encode la grille depuis les chaînes ci-dessus et compte `dots_total`.

---

## Spécifications de renderer.h/.c

```c
void draw_maze(SDL_Renderer *renderer, const Maze *maze);
```

Rendu en rectangles colorés (pas encore de sprites) :
- `CELL_WALL`         → rectangle bleu foncé `(33, 33, 222)`
- `CELL_DOT`          → petit rectangle blanc centré `(4×4 px)`, couleur `(255, 255, 255)`
- `CELL_POWER_PELLET` → rectangle blanc `(10×10 px)` centré dans la case
- `CELL_GHOST_DOOR`   → rectangle rose `(255, 182, 193)`
- `CELL_EMPTY`        → rien (fond noir)

Chaque case fait `CELL_SIZE` (16) pixels. La case `(col, row)` est à la position
pixel `(col * CELL_SIZE, row * CELL_SIZE)`.

---

## Modifications de src/main.c

Ajouter dans `AppState` :
```c
#include "game/maze.h"
#include "render/renderer.h"

typedef struct {
    SDL_Window   *window;
    SDL_Renderer *renderer;
    Maze          maze;
} AppState;
```

Dans `SDL_AppInit` : appeler `maze_init(&state->maze)`.

Dans `SDL_AppIterate` : appeler `draw_maze(state->renderer, &state->maze)`
après `SDL_RenderClear` et avant `SDL_RenderPresent`.

---

## Modifications de CMakeLists.txt

Remplacer :
```cmake
add_executable(Projet WIN32 src/main.c)
```
par :
```cmake
add_executable(Projet WIN32
    src/main.c
    src/game/maze.c
    src/render/renderer.c
)
```

---

## Ce que tu ne dois PAS faire

- Ne pas implémenter Pac-Man, les fantômes, le score ou les inputs
- Ne pas utiliser de sprites ou de textures SDL — uniquement `SDL_RenderFillRect`
- Ne pas ajouter de logique de jeu dans renderer.c

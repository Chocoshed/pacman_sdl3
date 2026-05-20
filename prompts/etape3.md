Tu développes un jeu Pac-Man en C avec SDL3.

## Contexte du projet

- Langage : C23
- Build : CMake, exécutable `Projet`, `SDL_MAIN_USE_CALLBACKS` activé
- Fenêtre : 448×576 px — 28 cases × 16 px de large, 36 cases × 16 px de haut
- Étapes 1 et 2 terminées : labyrinthe affiché en rectangles colorés

## État actuel du code

### src/main.c
```c
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include "game/maze.h"
#include "render/renderer.h"

#define WINDOW_WIDTH  448
#define WINDOW_HEIGHT 576

typedef struct {
    SDL_Window   *window;
    SDL_Renderer *renderer;
    Maze          maze;
} AppState;

SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[]);
SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event);
SDL_AppResult SDL_AppIterate(void *appstate);
void          SDL_AppQuit(void *appstate, SDL_AppResult result);
```

### src/game/maze.h
```c
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
```

### src/render/renderer.h
```c
#pragma once
#include <SDL3/SDL.h>
#include "../game/maze.h"

void draw_maze(SDL_Renderer *renderer, const Maze *maze);
```

## Tâche : Étape 3 — Pac-Man et déplacement

Crée les fichiers suivants :
- `src/input/input.h`
- `src/input/input.c`
- `src/game/pacman.h`
- `src/game/pacman.c`

Modifie :
- `src/render/renderer.h` — ajouter `draw_pacman()`
- `src/render/renderer.c` — implémenter `draw_pacman()`
- `src/main.c` — intégrer input, pacman, draw_pacman
- `CMakeLists.txt` — ajouter les nouveaux fichiers

---

## Spécifications de input.h/.c

### Types

```c
typedef enum {
    DIR_NONE  = 0,
    DIR_UP    = 1,
    DIR_DOWN  = 2,
    DIR_LEFT  = 3,
    DIR_RIGHT = 4,
} Direction;

typedef struct {
    Direction requested;   /* dernière direction demandée par le joueur */
} InputState;
```

### Fonctions

```c
void      input_init(InputState *input);
void      input_handle_event(InputState *input, const SDL_Event *event);
Direction input_get_direction(const InputState *input);
```

- `input_handle_event` : sur `SDL_EVENT_KEY_DOWN`, mettre à jour `requested`
  - Flèche haut / W → DIR_UP
  - Flèche bas  / S → DIR_DOWN
  - Flèche gauche / A → DIR_LEFT
  - Flèche droite / D → DIR_RIGHT

---

## Spécifications de pacman.h/.c

### Positions de départ

Pac-Man démarre à la case (col=14, row=26) — milieu du labyrinthe, côté joueur.

### Structure

```c
typedef struct {
    int       col;           /* position tile courante */
    int       row;
    Direction dir_current;   /* direction de déplacement actuelle */
    Direction dir_buffered;  /* prochaine direction demandée */
    float     move_timer;    /* accumulateur de temps entre déplacements */
} Pacman;
```

### Constante de vitesse

```c
#define PACMAN_SPEED 0.15f   /* secondes entre chaque déplacement d'une case */
```

### Fonctions

```c
void pacman_init(Pacman *pacman);
void pacman_update(Pacman *pacman, const Maze *maze, Direction input, float delta_time);
```

### Logique de pacman_update

1. Stocker `input` dans `dir_buffered` si `input != DIR_NONE`
2. Accumuler `delta_time` dans `move_timer`
3. Quand `move_timer >= PACMAN_SPEED` :
   - Réinitialiser `move_timer`
   - Calculer la case cible en appliquant `dir_buffered` depuis la position courante
   - Si cette case n'est pas un mur → appliquer `dir_buffered` comme nouvelle direction
   - Sinon → tenter de continuer avec `dir_current`
   - Déplacer si la case cible finale n'est pas un mur
4. **Tunnel** : si `col < 0` → `col = MAZE_COLS - 1`, si `col >= MAZE_COLS` → `col = 0`
5. Ne pas traverser `CELL_WALL` ni `CELL_GHOST_DOOR`

---

## Modifications de renderer.h/.c

Ajouter la déclaration dans `renderer.h` :
```c
#include "../game/pacman.h"

void draw_pacman(SDL_Renderer *renderer, const Pacman *pacman);
```

Implémenter dans `renderer.c` :
- Rectangle jaune `(255, 255, 0)` de taille `CELL_SIZE × CELL_SIZE`
- Positionné à `(pacman->col * CELL_SIZE, pacman->row * CELL_SIZE)`

---

## Modifications de src/main.c

Ajouter dans `AppState` :
```c
#include "input/input.h"
#include "game/pacman.h"

typedef struct {
    SDL_Window   *window;
    SDL_Renderer *renderer;
    Maze          maze;
    InputState    input;
    Pacman        pacman;
} AppState;
```

Dans `SDL_AppInit` : appeler `input_init` et `pacman_init`.

Dans `SDL_AppEvent` : appeler `input_handle_event(&state->input, event)`.

Dans `SDL_AppIterate` :
- Calculer `delta_time` avec `SDL_GetTicks()` (millisecondes → secondes)
- Appeler `pacman_update(&state->pacman, &state->maze, input_get_direction(&state->input), delta_time)`
- Appeler `draw_pacman(state->renderer, &state->pacman)` après `draw_maze`

---

## Modifications de CMakeLists.txt

```cmake
add_executable(Projet WIN32
    src/main.c
    src/game/maze.c
    src/game/pacman.c
    src/render/renderer.c
    src/input/input.c
)
```

---

## Ce que tu ne dois PAS faire

- Ne pas implémenter les fantômes, le score ou les fruits
- Ne pas utiliser de sprites — uniquement `SDL_RenderFillRect`
- Ne pas ajouter d'animation de Pac-Man (bouche) — juste un rectangle jaune
- Ne pas modifier la logique du labyrinthe

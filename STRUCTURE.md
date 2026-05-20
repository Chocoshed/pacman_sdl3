# Structure du projet Pac-Man SDL3

```
pacman_sdl3/
├── CMakeLists.txt
├── assets/
│   └── sprites.bmp          # spritesheet BMP (Pac-Man, fantômes, fruits, HUD)
└── src/
    ├── main.c               # SDL_AppInit/Event/Iterate/Quit — point d'entrée uniquement
    │
    ├── game/
    │   ├── game.h/.c        # AppState (TITLE/MENU/PLAY/PAUSE/GAME_OVER), MenuState
    │   ├── maze.h/.c        # Grille 28×36, is_wall(), eat_dot(), dots_remaining()
    │   │                    # spawn/despawn fruit, timer de disparition
    │   ├── pacman.h/.c      # Position, direction, buffering input, animation
    │   ├── ghost.h/.c       # 4 fantômes, AI via pointeur de fonction (update_target)
    │   └── score.h/.c       # Score, high score, vies, niveau, valeur fruits par niveau
    │
    ├── render/
    │   ├── sprite.h/.c      # Chargement BMP, spritesheet, animation par frame
    │   └── renderer.h/.c    # draw_title(), draw_menu(), draw_maze(), draw_pacman(),
    │                        # draw_ghost(), draw_fruit(), draw_hud()
    │
    └── input/
        └── input.h/.c       # État clavier, direction bufferisée, navigation menu
```

## Responsabilités par module

### `main.c`
Point d'entrée SDL3 uniquement. Initialise l'app, délègue tout au `GameState`.

### `game/game.h/.c`
- Machine d'états principale : `TITLE → MENU → PLAY → PAUSE → GAME_OVER`
- `MenuState` : option sélectionnée (New Game, Quit…)
- Coordonne les appels entre tous les sous-modules

### `game/maze.h/.c`
- Grille statique 28×36 cases (murs, couloirs, dots, power pellets)
- Requêtes : `is_wall()`, `eat_dot()`, `dots_remaining()`
- Gestion du fruit : spawn aux seuils 70 et 170 dots mangés, timer ~10s, `eat_fruit()`

### `game/pacman.h/.c`
- Position tile + position pixel interpolée
- Direction courante + direction bufferisée (input anticipé)
- Frame d'animation (bouche ouverte/fermée)

### `game/ghost.h/.c`
- Structure commune aux 4 fantômes (Blinky, Pinky, Inky, Clyde)
- Mode : `SCATTER`, `CHASE`, `FRIGHTENED`, `EATEN`
- AI via pointeur de fonction `update_target()` — une stratégie par fantôme

### `game/score.h/.c`
- Score courant, high score, nombre de vies, niveau
- Valeur des fruits par niveau (cerise=100, fraise=300, orange=500…)
- Bonus combo fantômes mangés en mode frightened (200, 400, 800, 1600)

### `render/sprite.h/.c`
- Chargement du BMP, découpe de la spritesheet en rectangles source
- Système d'animation : frame courante, delta time, vitesse par animation

### `render/renderer.h/.c`
- Toutes les fonctions de dessin : `draw_title()`, `draw_menu()`, `draw_maze()`,
  `draw_pacman()`, `draw_ghost()`, `draw_fruit()`, `draw_hud()`
- Ne contient aucune logique métier — lit l'état, affiche

### `input/input.h/.c`
- Snapshot de l'état clavier à chaque frame
- Direction bufferisée pour Pac-Man (mémorise la prochaine direction demandée)
- Navigation dans les menus (haut/bas/entrée)

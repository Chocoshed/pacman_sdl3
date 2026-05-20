# Étapes de développement — Pac-Man SDL3

> **Règle :** chaque étape doit compiler et tourner avant de passer à la suivante.

---

## Étape 1 — Squelette SDL3

**Objectif :** fenêtre noire qui s'ouvre et se ferme proprement.

- `main.c` : implémenter `SDL_AppInit`, `SDL_AppEvent`, `SDL_AppIterate`, `SDL_AppQuit`
- `SDL_AppInit` : créer la fenêtre (448×576 px, ratio 28×36 cases de 16px) et le renderer
- `SDL_AppEvent` : quitter sur `SDL_EVENT_QUIT` et touche `Escape`
- `SDL_AppIterate` : `SDL_RenderClear` + `SDL_RenderPresent`

**Critère de validation :** fenêtre noire qui répond à Escape.

---

## Étape 2 — Labyrinthe

**Objectif :** afficher la grille du labyrinthe en rectangles colorés.

- `maze.h/.c` :
  - Définir les types de cases : `WALL`, `DOT`, `POWER_PELLET`, `EMPTY`, `FRUIT_SPAWN`
  - Grille 28×36 en tableau statique `uint8_t maze[36][28]`
  - Encoder le labyrinthe classique de Pac-Man
  - Implémenter `is_wall(x, y)`, `eat_dot(x, y)`, `dots_remaining()`
- `renderer.h/.c` :
  - `draw_maze()` : rectangles bleus pour les murs, points blancs pour les dots,
    grands cercles blancs pour les power pellets

**Critère de validation :** labyrinthe complet affiché à l'écran.

---

## Étape 3 — Pac-Man et déplacement

**Objectif :** Pac-Man se déplace dans le labyrinthe avec collisions murs.

- `input.h/.c` :
  - Snapshot clavier chaque frame
  - `get_input_direction()` : retourne la direction demandée (HAUT/BAS/GAUCHE/DROITE)
- `pacman.h/.c` :
  - Structure `Pacman` : position tile (x, y), direction courante, direction bufferisée
  - `pacman_update()` : tenter d'appliquer la direction bufferisée, sinon continuer
    la direction courante, bloquer sur les murs
  - Tunnel gauche/droite (côtés du labyrinthe)
- `renderer.h/.c` :
  - `draw_pacman()` : rectangle jaune (pas encore de sprite)

**Critère de validation :** Pac-Man se déplace, ne traverse pas les murs,
le buffering permet d'anticiper les virages.

---

## Étape 4 — Dots et score de base

**Objectif :** Pac-Man mange les dots et le score s'incrémente.

- `score.h/.c` :
  - Structure `Score` : points courants, high score, vies, niveau
  - `add_points(n)`, `lose_life()`, `next_level()`
- Connecter `eat_dot()` dans `pacman_update()` → `add_points(10)`
- Power pellets : `add_points(50)`, déclencher le mode frightened (placeholder)
- `renderer.h/.c` : `draw_hud()` : score, high score, vies en haut de l'écran

**Critère de validation :** le score augmente en mangeant, les dots disparaissent.

---

## Étape 5 — Fruits

**Objectif :** le fruit apparaît, Pac-Man peut le manger pour des points bonus.

- `maze.h/.c` :
  - Compteur de dots mangés dans l'état du labyrinthe
  - `try_spawn_fruit()` : active le fruit aux seuils 70 et 170 dots mangés
  - Timer de disparition (~10 secondes), `despawn_fruit()`
  - `eat_fruit()` : désactive le fruit, retourne la valeur en points
- `score.h/.c` :
  - Table `fruit_value[niveau]` : cerise=100, fraise=300, orange=500,
    pomme=700, melon=1000, galaxian=2000, cloche=3000, clé=5000
- `renderer.h/.c` : `draw_fruit()` : rectangle coloré selon le niveau (pas encore de sprite)

**Critère de validation :** le fruit apparaît deux fois par niveau, disparaît après ~10s,
rapporte les bons points.

---

## Étape 6 — Fantômes (déplacement basique)

**Objectif :** 4 fantômes se déplacent dans le labyrinthe.

- `ghost.h/.c` :
  - Structure `Ghost` : position, direction, mode (`SCATTER`/`CHASE`/`FRIGHTENED`/`EATEN`)
  - Déplacement tile par tile, pas de demi-tour autorisé sauf changement de mode
  - AI placeholder : cible aléatoire parmi les cases accessibles
  - `ghost_update()` pour chaque fantôme
- `renderer.h/.c` : `draw_ghost()` : rectangles de couleur (un par fantôme)

**Critère de validation :** les 4 fantômes circulent sans traverser les murs.

---

## Étape 7 — Collisions et règles du jeu

**Objectif :** le jeu a des conditions de victoire et de défaite.

- Collision Pac-Man / fantôme :
  - Mode normal → `lose_life()`, reset positions, animation de mort (placeholder)
  - Mode frightened → fantôme passe en mode `EATEN`, `add_points(combo)`
- Combo fantômes : 200, 400, 800, 1600 points (reset à chaque power pellet)
- Condition victoire : `dots_remaining() == 0` → `next_level()`
- Condition game over : `vies == 0` → transition vers `GAME_OVER`

**Critère de validation :** le jeu peut se gagner et se perdre.

---

## Étape 8 — Machine d'états et menus

**Objectif :** écrans titre, menu, pause et game over fonctionnels.

- `game.h/.c` :
  - `AppState` : `TITLE`, `MENU`, `PLAY`, `PAUSE`, `GAME_OVER`
  - `MenuState` : option sélectionnée, navigation haut/bas
  - Transitions : titre → menu sur entrée, menu → jeu, jeu → pause sur P,
    game over → menu
- `input.h/.c` : navigation menu (haut/bas/entrée)
- `renderer.h/.c` :
  - `draw_title()` : logo Pac-Man, "Press Enter"
  - `draw_menu()` : options New Game / Quit avec curseur
  - `draw_game_over()` : score final, high score

**Critère de validation :** navigation complète entre tous les écrans.

---

## Étape 9 — Sprites

**Objectif :** remplacer tous les rectangles par les vrais sprites.

- `sprite.h/.c` :
  - `sprite_load()` : charger le BMP, créer la texture SDL
  - `Spritesheet` : tableau de `SDL_FRect` source indexés par frame
  - `animation_update(delta_time)` : avancer la frame selon la vitesse de l'animation
- `renderer.h/.c` : remplacer chaque `SDL_RenderFillRect` par `SDL_RenderTexture`
  avec la bonne région source
- Animations à implémenter :
  - Pac-Man : bouche ouverte → fermée (4 frames × 4 directions)
  - Fantômes : 2 frames par direction, 2 frames frightened, 2 frames eaten
  - Fruits : 1 frame par type

**Critère de validation :** tous les sprites s'affichent et s'animent correctement.

---

## Étape 10 — AI des fantômes

**Objectif :** chaque fantôme a une personnalité propre.

- `ghost.h/.c` : remplacer l'AI aléatoire par des pointeurs de fonction `update_target()`
- Implémentation par fantôme :
  - **Blinky** (rouge) : cible = position exacte de Pac-Man
  - **Pinky** (rose) : cible = 4 cases devant Pac-Man
  - **Inky** (cyan) : cible = symétrie Blinky / 2 cases devant Pac-Man
  - **Clyde** (orange) : cible = Pac-Man si > 8 cases, sinon coin bas-gauche
- Mode `SCATTER` : chaque fantôme rejoint son coin attitré
- Alternance `SCATTER`/`CHASE` selon un timer de niveau

**Critère de validation :** comportement distinct et reconnaissable pour chaque fantôme.

---

## Étape 11 — Polish

**Objectif :** expérience de jeu complète.

- Animation de mort de Pac-Man (rotation de sprite)
- Affichage du score gagné sur place (fantôme mangé, fruit)
- Clignotement du labyrinthe en fin de niveau
- Vitesses progressives selon le niveau (Pac-Man et fantômes)
- Sons via SDL_mixer (optionnel)
- High score persistant en fichier (optionnel)

Tu développes un jeu Pac-Man en C avec SDL3.

## Contexte du projet

- Langage : C23
- Build : CMake avec SDL3 (via FetchContent si non installé localement)
- SDL3 utilise le modèle de callbacks : SDL_AppInit, SDL_AppEvent, SDL_AppIterate, SDL_AppQuit
- Le CMakeLists.txt compile `src/main.c` en exécutable `Projet` avec SDL_MAIN_USE_CALLBACKS

## Tâche : Étape 1 — Squelette SDL3

Crée le fichier `src/main.c` uniquement.

### Exigences

- Fenêtre de 448×576 pixels (28 cases × 16px de large, 36 cases × 16px de haut)
- Titre de la fenêtre : "Pac-Man"
- Fond noir à chaque frame (SDL_RenderClear + SDL_RenderPresent)
- Quitter proprement sur SDL_EVENT_QUIT et sur la touche Escape
- Stocker la fenêtre et le renderer dans une struct AppState définie localement dans main.c
  (elle sera déplacée dans game.h à l'étape suivante)

### Signatures SDL3 à utiliser

```c
SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[]);
SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event);
SDL_AppResult SDL_AppIterate(void *appstate);
void          SDL_AppQuit(void *appstate, SDL_AppResult result);
```

- Retourner `SDL_APP_CONTINUE` pour continuer, `SDL_APP_SUCCESS` pour quitter proprement
- Allouer AppState avec SDL_malloc dans SDL_AppInit, libérer dans SDL_AppQuit

### Ce que tu ne dois PAS faire

- Ne pas créer d'autres fichiers que src/main.c
- Ne pas implémenter de logique de jeu
- Ne pas inclure de headers autres que `<SDL3/SDL.h>` et `<SDL3/SDL_main.h>`

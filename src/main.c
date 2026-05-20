#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include "game/maze.h"
#include "game/pacman.h"
#include "game/score.h"
#include "input/input.h"
#include "render/renderer.h"

#define WINDOW_WIDTH  448
#define WINDOW_HEIGHT 576

typedef struct {
    SDL_Window   *window;
    SDL_Renderer *renderer;
    Maze          maze;
    InputState    input;
    Pacman        pacman;
    Score         score;
    Uint64        last_ticks;
} AppState;

SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[]) {
    (void)argc;
    (void)argv;

    if (!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_Log("SDL_Init failed: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    AppState *state = SDL_malloc(sizeof(AppState));
    if (!state)
        return SDL_APP_FAILURE;

    if (!SDL_CreateWindowAndRenderer("Pac-Man", WINDOW_WIDTH, WINDOW_HEIGHT, 0,
                                     &state->window, &state->renderer)) {
        SDL_Log("SDL_CreateWindowAndRenderer failed: %s", SDL_GetError());
        SDL_free(state);
        return SDL_APP_FAILURE;
    }

    maze_init(&state->maze);
    input_init(&state->input);
    pacman_init(&state->pacman);
    score_init(&state->score);
    state->last_ticks = SDL_GetTicks();

    *appstate = state;
    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event) {
    AppState *state = (AppState *)appstate;

    if (event->type == SDL_EVENT_QUIT)
        return SDL_APP_SUCCESS;

    if (event->type == SDL_EVENT_KEY_DOWN &&
        event->key.scancode == SDL_SCANCODE_ESCAPE)
        return SDL_APP_SUCCESS;

    input_handle_event(&state->input, event);
    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppIterate(void *appstate) {
    AppState *state = (AppState *)appstate;

    Uint64 now        = SDL_GetTicks();
    float  delta_time = (float)(now - state->last_ticks) / 1000.0f;
    state->last_ticks = now;

    pacman_update(&state->pacman, &state->maze, &state->score,
                  input_get_direction(&state->input), delta_time);

    SDL_SetRenderDrawColor(state->renderer, 0, 0, 0, 255);
    SDL_RenderClear(state->renderer);
    draw_maze(state->renderer, &state->maze);
    draw_pacman(state->renderer, &state->pacman);
    draw_hud(state->renderer, &state->score);
    SDL_RenderPresent(state->renderer);

    return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void *appstate, SDL_AppResult result) {
    (void)result;

    AppState *state = (AppState *)appstate;
    if (state) {
        SDL_DestroyRenderer(state->renderer);
        SDL_DestroyWindow(state->window);
        SDL_free(state);
    }
    SDL_Quit();
}

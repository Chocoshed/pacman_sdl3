#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include "game/game.h"
#include "game/maze.h"
#include "game/pacman.h"
#include "game/score.h"
#include "game/ghost.h"
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
    Ghosts        ghosts;
    MenuState     menu;
    GamePhase     phase;
    float         phase_timer;
    Uint64        last_ticks;
} AppState;

static void start_new_game(AppState *state) {
    score_init(&state->score);
    maze_init(&state->maze);
    game_reset_positions(&state->pacman, &state->ghosts);
    state->phase       = PHASE_PLAY;
    state->phase_timer = 0.0f;
}

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
    ghosts_init(&state->ghosts);
    state->menu.selected = MENU_NEW_GAME;
    state->phase         = PHASE_TITLE;
    state->phase_timer   = 0.0f;
    state->last_ticks    = SDL_GetTicks();

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

    SDL_AppResult result = SDL_APP_CONTINUE;

    switch (state->phase) {

        case PHASE_TITLE:
            if (state->input.confirm)
                state->phase = PHASE_MENU;
            break;

        case PHASE_MENU:
            if (state->input.nav_up)
                state->menu.selected = MENU_NEW_GAME;
            if (state->input.nav_down)
                state->menu.selected = MENU_QUIT;
            if (state->input.confirm) {
                if (state->menu.selected == MENU_NEW_GAME)
                    start_new_game(state);
                else
                    result = SDL_APP_SUCCESS;
            }
            break;

        case PHASE_PLAY: {
            pacman_update(&state->pacman, &state->maze, &state->score,
                          input_get_direction(&state->input), delta_time);

            if (state->score.power_pellet_eaten) {
                state->score.power_pellet_eaten = false;
                state->score.ghost_combo        = 0;
                ghosts_set_frightened(&state->ghosts);
            }

            ghosts_update(&state->ghosts, &state->maze, &state->pacman, delta_time);
            maze_update_fruit(&state->maze, delta_time);

            if (maze_try_eat_fruit(&state->maze, state->pacman.col, state->pacman.row))
                score_add_points(&state->score, score_fruit_value(state->score.level));

            GamePhase prev = state->phase;
            game_check_collisions(&state->phase, &state->pacman, &state->ghosts, &state->score);
            if (state->phase != prev)
                state->phase_timer = DEATH_PAUSE_DURATION;

            if (state->phase == PHASE_PLAY && maze_dots_remaining(&state->maze) == 0) {
                state->phase       = PHASE_LEVEL_CLEAR;
                state->phase_timer = LEVEL_PAUSE_DURATION;
            }

            if (state->input.pause)
                state->phase = PHASE_PAUSE;
            break;
        }

        case PHASE_PAUSE:
            if (state->input.pause)
                state->phase = PHASE_PLAY;
            break;

        case PHASE_DYING:
        case PHASE_LEVEL_CLEAR:
            state->phase_timer -= delta_time;
            if (state->phase_timer <= 0.0f) {
                if (state->phase == PHASE_LEVEL_CLEAR) {
                    score_next_level(&state->score);
                    maze_init(&state->maze);
                }
                game_reset_positions(&state->pacman, &state->ghosts);
                state->phase = PHASE_PLAY;
            }
            break;

        case PHASE_GAME_OVER:
            if (state->input.confirm) {
                state->menu.selected = MENU_NEW_GAME;
                state->phase         = PHASE_MENU;
            }
            break;
    }

    input_clear_actions(&state->input);

    SDL_SetRenderDrawColor(state->renderer, 0, 0, 0, 255);
    SDL_RenderClear(state->renderer);

    switch (state->phase) {
        case PHASE_TITLE:
            draw_title(state->renderer);
            break;

        case PHASE_MENU:
            draw_menu(state->renderer, (int)state->menu.selected);
            break;

        case PHASE_PLAY:
        case PHASE_DYING:
        case PHASE_LEVEL_CLEAR:
            draw_maze(state->renderer, &state->maze);
            draw_pacman(state->renderer, &state->pacman);
            for (int i = 0; i < GHOST_COUNT; i++)
                draw_ghost(state->renderer, &state->ghosts.ghosts[i]);
            draw_fruit(state->renderer, &state->maze, state->score.level);
            draw_hud(state->renderer, &state->score);
            break;

        case PHASE_PAUSE:
            draw_maze(state->renderer, &state->maze);
            draw_pacman(state->renderer, &state->pacman);
            for (int i = 0; i < GHOST_COUNT; i++)
                draw_ghost(state->renderer, &state->ghosts.ghosts[i]);
            draw_fruit(state->renderer, &state->maze, state->score.level);
            draw_hud(state->renderer, &state->score);
            draw_pause(state->renderer);
            break;

        case PHASE_GAME_OVER:
            draw_maze(state->renderer, &state->maze);
            draw_pacman(state->renderer, &state->pacman);
            for (int i = 0; i < GHOST_COUNT; i++)
                draw_ghost(state->renderer, &state->ghosts.ghosts[i]);
            draw_fruit(state->renderer, &state->maze, state->score.level);
            draw_hud(state->renderer, &state->score);
            draw_game_over(state->renderer, &state->score);
            break;
    }

    SDL_RenderPresent(state->renderer);
    return result;
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

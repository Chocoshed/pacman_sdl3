#pragma once

#include <SDL3/SDL.h>
#include "../types.h"

/*
 * ISP : ce header ne concerne que la mécanique d'entrée clavier.
 * Direction est défini dans types.h, séparé de InputState.
 */

typedef struct {
    Direction requested;
    bool      confirm;
    bool      pause;
    bool      nav_up;
    bool      nav_down;
} InputState;

void      input_init(InputState *input);
void      input_clear_actions(InputState *input);
void      input_handle_event(InputState *input, const SDL_Event *event);
Direction input_get_direction(const InputState *input);

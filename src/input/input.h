#pragma once

#include <SDL3/SDL.h>

typedef enum {
    DIR_NONE  = 0,
    DIR_UP    = 1,
    DIR_DOWN  = 2,
    DIR_LEFT  = 3,
    DIR_RIGHT = 4,
} Direction;

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

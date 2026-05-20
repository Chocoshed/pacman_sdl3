#include "input.h"

void input_init(InputState *input) {
    input->requested = DIR_NONE;
}

void input_handle_event(InputState *input, const SDL_Event *event) {
    if (event->type != SDL_EVENT_KEY_DOWN)
        return;

    switch (event->key.scancode) {
        case SDL_SCANCODE_UP:    case SDL_SCANCODE_W: input->requested = DIR_UP;    break;
        case SDL_SCANCODE_DOWN:  case SDL_SCANCODE_S: input->requested = DIR_DOWN;  break;
        case SDL_SCANCODE_LEFT:  case SDL_SCANCODE_A: input->requested = DIR_LEFT;  break;
        case SDL_SCANCODE_RIGHT: case SDL_SCANCODE_D: input->requested = DIR_RIGHT; break;
        default: break;
    }
}

Direction input_get_direction(const InputState *input) {
    return input->requested;
}

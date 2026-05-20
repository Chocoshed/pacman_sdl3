#include "input.h"

void input_init(InputState *input) {
    input->requested = DIR_NONE;
    input->confirm   = false;
    input->pause     = false;
    input->nav_up    = false;
    input->nav_down  = false;
}

void input_clear_actions(InputState *input) {
    input->confirm  = false;
    input->pause    = false;
    input->nav_up   = false;
    input->nav_down = false;
}

void input_handle_event(InputState *input, const SDL_Event *event) {
    if (event->type != SDL_EVENT_KEY_DOWN)
        return;

    switch (event->key.scancode) {
        case SDL_SCANCODE_UP:
        case SDL_SCANCODE_W:
            input->requested = DIR_UP;
            input->nav_up    = true;
            break;
        case SDL_SCANCODE_DOWN:
        case SDL_SCANCODE_S:
            input->requested = DIR_DOWN;
            input->nav_down  = true;
            break;
        case SDL_SCANCODE_LEFT:
        case SDL_SCANCODE_A:
            input->requested = DIR_LEFT;
            break;
        case SDL_SCANCODE_RIGHT:
        case SDL_SCANCODE_D:
            input->requested = DIR_RIGHT;
            break;
        case SDL_SCANCODE_RETURN:
        case SDL_SCANCODE_RETURN2:
        case SDL_SCANCODE_KP_ENTER:
            input->confirm = true;
            break;
        case SDL_SCANCODE_P:
            input->pause = true;
            break;
        default:
            break;
    }
}

Direction input_get_direction(const InputState *input) {
    return input->requested;
}

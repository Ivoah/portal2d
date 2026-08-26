#include "Input.h"

InputButton Input_fromEvent(SDL_Event* event, bool inMenu) {
    switch (event->type) {
        case SDL_EVENT_KEY_DOWN:
            switch (event->key.scancode) {
                case SDL_SCANCODE_ESCAPE: return I_BACK;
                case SDL_SCANCODE_SPACE:
                case SDL_SCANCODE_RETURN: return I_CONFIRM;
                case SDL_SCANCODE_UP:
                case SDL_SCANCODE_W:      return I_UP;
                case SDL_SCANCODE_DOWN:
                case SDL_SCANCODE_S:      return I_DOWN;
                case SDL_SCANCODE_LEFT:
                case SDL_SCANCODE_A:      return I_LEFT;
                case SDL_SCANCODE_RIGHT:
                case SDL_SCANCODE_D:      return I_RIGHT;
                case SDL_SCANCODE_I:      return I_SHOOT_UP;
                case SDL_SCANCODE_K:      return I_SHOOT_DOWN;
                case SDL_SCANCODE_J:      return I_SHOOT_LEFT;
                case SDL_SCANCODE_L:      return I_SHOOT_RIGHT;
                default: break;
            }
            break;
        case SDL_EVENT_GAMEPAD_BUTTON_DOWN:
            switch (event->gbutton.button) {
                case SDL_GAMEPAD_BUTTON_DPAD_UP:        return                      I_UP;
                case SDL_GAMEPAD_BUTTON_DPAD_DOWN:      return                      I_DOWN;
                case SDL_GAMEPAD_BUTTON_DPAD_LEFT:      return                      I_LEFT;
                case SDL_GAMEPAD_BUTTON_DPAD_RIGHT:     return                      I_RIGHT;
                case SDL_GAMEPAD_BUTTON_NORTH:          return                      I_SHOOT_UP;
                case SDL_GAMEPAD_BUTTON_SOUTH:          return inMenu ? I_CONFIRM : I_SHOOT_DOWN;
                case SDL_GAMEPAD_BUTTON_WEST:           return                      I_SHOOT_LEFT;
                case SDL_GAMEPAD_BUTTON_EAST:           return inMenu ? I_BACK    : I_SHOOT_RIGHT;
                case SDL_GAMEPAD_BUTTON_START:          return inMenu ? I_CONFIRM : I_BACK;
                default: break;
            }
            break;
    }

    return I_UNKNOWN;
}

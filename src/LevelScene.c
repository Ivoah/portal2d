#include "LevelScene.h"
#include "Level.h"
#include "MenuScene.h"
#include "Util.h"
#include "Event.h"

typedef struct {
    SDL_Renderer* renderer;
    SDL_Texture* tiles;
    Level* level;
} LevelSceneState;

SDL_AppResult LevelScene_event(Scene* scene, SDL_Event* event) {
    LevelSceneState* lss = (LevelSceneState*)scene->state;
    
    int levelToLoad = -1;
    Vec2* moveDir = NULL;
    Vec2* shotDir = NULL;

    switch (event->type) {
        case SDL_EVENT_KEY_DOWN:
            switch (event->key.scancode) {
                case SDL_SCANCODE_ESCAPE:
                    SDL_PushEvent(&(SDL_Event){.user.type = EVENT_LOAD_SCENE, .user.data1 = MenuScene_create(lss->renderer, lss->level->levelNum, 0)});
                    return SDL_APP_CONTINUE;
                case SDL_SCANCODE_UP:
                case SDL_SCANCODE_W:      moveDir = &V_UP; break;
                case SDL_SCANCODE_DOWN:
                case SDL_SCANCODE_S:      moveDir = &V_DOWN; break;
                case SDL_SCANCODE_LEFT:
                case SDL_SCANCODE_A:      moveDir = &V_LEFT; break;
                case SDL_SCANCODE_RIGHT:
                case SDL_SCANCODE_D:      moveDir = &V_RIGHT; break;
                case SDL_SCANCODE_I:      shotDir = &V_UP; break;
                case SDL_SCANCODE_K:      shotDir = &V_DOWN; break;
                case SDL_SCANCODE_J:      shotDir = &V_LEFT; break;
                case SDL_SCANCODE_L:      shotDir = &V_RIGHT; break;
                default: break;
            }
            switch (event->key.key) {
                case SDLK_R:      Level_undo(lss->level, -1); break;
                case SDLK_COMMA:  levelToLoad = lss->level->levelNum - 1; break;
                case SDLK_PERIOD: levelToLoad = lss->level->levelNum + 1; break;
                case SDLK_U:      Level_undo(lss->level, 1); break;
                default: break;
            }
            break;
        case SDL_EVENT_GAMEPAD_BUTTON_DOWN:
            switch (event->gbutton.button) {
                case SDL_GAMEPAD_BUTTON_DPAD_UP:    moveDir = &V_UP; break;
                case SDL_GAMEPAD_BUTTON_DPAD_DOWN:  moveDir = &V_DOWN; break;
                case SDL_GAMEPAD_BUTTON_DPAD_LEFT:  moveDir = &V_LEFT; break;
                case SDL_GAMEPAD_BUTTON_DPAD_RIGHT: moveDir = &V_RIGHT; break;
                case SDL_GAMEPAD_BUTTON_NORTH:      shotDir = &V_UP; break;
                case SDL_GAMEPAD_BUTTON_SOUTH:      shotDir = &V_DOWN; break;
                case SDL_GAMEPAD_BUTTON_WEST:       shotDir = &V_LEFT; break;
                case SDL_GAMEPAD_BUTTON_EAST:       shotDir = &V_RIGHT; break;
                case SDL_GAMEPAD_BUTTON_BACK:       Level_undo(lss->level, 1); break;
                case SDL_GAMEPAD_BUTTON_START:
                    SDL_PushEvent(&(SDL_Event){.user.type = EVENT_LOAD_SCENE, .user.data1 = MenuScene_create(lss->renderer, lss->level->levelNum, 0)});
                    return SDL_APP_CONTINUE;
                default: break;
            }
            break;
    }

    if (moveDir != NULL) Level_move(lss->level, *moveDir);
    if (shotDir != NULL) Level_shoot(lss->level, *shotDir);

    // Advance level if won
    if (Level_isWon(lss->level)) {
        levelToLoad = lss->level->levelNum + 1;
    }

    if (levelToLoad != -1) {
        levelToLoad = levelToLoad%10;
        Level_free(lss->level);
        lss->level = Level_load(levelToLoad);
        if (lss->level == NULL) return SDL_APP_SUCCESS;
    }

    if (lss->level->state->lastShotBlue)
        SDL_SetGamepadLED(SDL_GetGamepadFromPlayerIndex(0), 0, 0, 255);
    else
        SDL_SetGamepadLED(SDL_GetGamepadFromPlayerIndex(0), 255, 35, 0);

    return SDL_APP_CONTINUE;
}

void LevelScene_draw(Scene* scene, SDL_Renderer* renderer) {
    LevelSceneState* lss = (LevelSceneState*)scene->state;

    Level_draw(renderer, lss->tiles, lss->level);
}

void LevelScene_free(Scene* scene) {
    LevelSceneState* lss = (LevelSceneState*)scene->state;

    SDL_DestroyTexture(lss->tiles);
    Level_free(lss->level);
    SDL_free(scene->state);
    SDL_free(scene);
}

Scene* LevelScene_create(SDL_Renderer* renderer, int level) {
    Scene* scene = SDL_calloc(1, sizeof(Scene));
    *scene = (Scene){
        .state = SDL_calloc(1, sizeof(LevelSceneState)),
        .event = LevelScene_event,
        .draw = LevelScene_draw,
        .free = LevelScene_free
    };

    *((LevelSceneState*)scene->state) = (LevelSceneState){
        .renderer = renderer,
        .tiles = Util_loadTexture(renderer, "tiles.png"),
        .level = Level_load(level),
    };

    return scene;
}

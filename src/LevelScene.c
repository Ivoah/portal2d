#include "LevelScene.h"

#include "Level.h"
#include "Input.h"
#include "Event.h"
#include "Util.h"

typedef struct {
    Level* level;
} LevelSceneState;

SDL_AppResult LevelScene_event(Scene* scene, SDL_Event* event) {
    LevelSceneState* lss = (LevelSceneState*)scene->state;
    
    switch (Input_fromEvent(event, false)) {
        case I_UP:          break;
        case I_DOWN:        break;
        case I_LEFT:        break;
        case I_RIGHT:       break;
        case I_SHOOT_UP:    break;
        case I_SHOOT_DOWN:  break;
        case I_SHOOT_LEFT:  break;
        case I_SHOOT_RIGHT: break;
        case I_BACK:        SDL_PushEvent(&(SDL_Event){.user.type = EVENT_POP_SCENE}); return SDL_APP_CONTINUE;
        default: break;
    }

    // if (lss->level->state->lastShotBlue)
    //     SDL_SetGamepadLED(SDL_GetGamepadFromPlayerIndex(0), 0, 0, 255);
    // else
    //     SDL_SetGamepadLED(SDL_GetGamepadFromPlayerIndex(0), 255, 35, 0);

    return SDL_APP_CONTINUE;
}

void LevelScene_update(Scene* scene, int delta) {
    LevelSceneState* lss = (LevelSceneState*)scene->state;

    Level_update(lss->level, delta);
}

void LevelScene_draw(Scene* scene, SDL_Renderer* renderer) {
    LevelSceneState* lss = (LevelSceneState*)scene->state;

    Level_draw(renderer, lss->level);
}

void LevelScene_free(Scene* scene) {
    LevelSceneState* lss = (LevelSceneState*)scene->state;

    Level_free(lss->level);
    SDL_free(scene->state);
    SDL_free(scene);
}

Scene* LevelScene_create(int levelNum) {
    Scene* scene = SDL_calloc(1, sizeof(Scene));
    *scene = (Scene){
        .state = SDL_calloc(1, sizeof(LevelSceneState)),
        .clearColor = {255, 255, 255},
        .event = LevelScene_event,
        .update = LevelScene_update,
        .draw = LevelScene_draw,
        .free = LevelScene_free
    };

    Level* level = Level_load(levelNum);
    if (level == NULL) {
        SDL_Log("Could not load level");
        return NULL;
    }

    *((LevelSceneState*)scene->state) = (LevelSceneState){
        .level = level
    };

    return scene;
}

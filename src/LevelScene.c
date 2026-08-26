#include "LevelScene.h"

#include "Level.h"
#include "Input.h"
#include "Event.h"
#include "Util.h"

typedef struct {
    SDL_Renderer* renderer;
    SDL_Texture* tiles;
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

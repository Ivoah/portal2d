#include "Menu.h"
#include "Text.h"
#include "Scene.h"
#include "Save.h"
#include "Util.h"
#include "main.h"

typedef struct {
    Uint8 level;
    int menuItem;
    SDL_Texture* logo;
    SDL_Texture* selector;
} MenuSceneState;

void Menu_free(void* sceneState) {
    MenuSceneState* mss = (MenuSceneState*)sceneState;
    
    SDL_DestroyTexture(mss->logo);
    SDL_DestroyTexture(mss->selector);
    SDL_free(sceneState);
}

SDL_AppResult Menu_event(void* sceneState, SDL_Event* event) {
    return SDL_APP_CONTINUE;
}

void Menu_draw(void* sceneState, SDL_Renderer* renderer) {
    MenuSceneState* mss = (MenuSceneState*)sceneState;

    char fmtStr[100];

    Util_drawWobbly(renderer, mss->logo, (Vec2){WINDOW_WIDTH/2 - mss->logo->w/2, 100});

    SDL_snprintf(fmtStr, sizeof(fmtStr)/sizeof(char), "Level <%02d>", mss->level);
    Text_drawCentered(renderer, fmtStr, 200);

    // int x = WINDOW_WIDTH/2 - (Text_getWidth(T_LEVEL) + T_TEXT_HEIGHT*4)/2;
    // Text_draw(renderer, T_LEVEL, (Vec2){x, 200}); x += Text_getWidth(T_LEVEL);
    // Text_draw(renderer, T_LT, (Vec2){x, 200}); x += Text_getWidth(T_LT);
    // Text_drawNumber(renderer, mss->level, 2, 2, (Vec2){x, 200}); x += T_TEXT_HEIGHT*2;
    // Text_draw(renderer, T_GT, (Vec2){x, 200});
}

Scene Menu_scene(SDL_Renderer* renderer) {
    MenuSceneState* mss = SDL_calloc(1, sizeof(MenuSceneState));

    mss->logo = Util_loadTexture(renderer, "logo.png");
    mss->selector = Util_loadTexture(renderer, "selector.png");

    return (Scene){
        .state = mss,
        .freeState = &Menu_free,
        .event = &Menu_event,
        .draw = &Menu_draw
    };
}

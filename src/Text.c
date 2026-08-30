#include "Text.h"

#include "Util.h"
#include "main.h"

#define T_NUM_CHARS ('}' - ' ')

static SDL_Texture* font = NULL;
static Vec4 Text_hitboxes[T_NUM_CHARS][2];

bool Text_init(SDL_Renderer* renderer) {
    SDL_Surface* fontSurface = Util_loadSurface("font.png");

    Text_hitboxes[0][0] = (Vec4){0, 0, T_SPACING, T_SIZE};
    Text_hitboxes[0][1] = (Vec4){0, 0, T_SPACING, T_SIZE};
    for (int i = 1; i < T_NUM_CHARS; i++) {
        Text_hitboxes[i][0] = Util_measureSprite(fontSurface, (Vec4){i*T_SIZE, 0, T_SIZE, T_SIZE});
        Text_hitboxes[i][1] = Util_measureSprite(fontSurface, (Vec4){i*T_SIZE, T_SIZE, T_SIZE, T_SIZE});
    }

    font = SDL_CreateTextureFromSurface(renderer, fontSurface);
    SDL_DestroySurface(fontSurface);

    return true;
}

void Text_deinit() {
    SDL_DestroyTexture(font);
}

void Text_draw(SDL_Renderer* renderer, const char* str, Vec2 pos) {
    int j = (SDL_GetTicks()/U_WOBBLE_SPEED)%2;
    SDL_FRect src_rect = {0, j*T_SIZE, T_SIZE, T_SIZE};
    SDL_FRect dst_rect = {pos.x, pos.y, T_SIZE, T_SIZE};

    for (const char* c = str; *c != '\0'; c += sizeof(char)) {
        int i = *c - ' ';
        src_rect.x = i*T_SIZE + Text_hitboxes[i][j].x;
        src_rect.w = Text_hitboxes[i][j].w;
        dst_rect.w = Text_hitboxes[i][j].w;
        SDL_RenderTexture(renderer, font, &src_rect, &dst_rect);
        dst_rect.x += Text_hitboxes[i][j].w + T_SPACING;
    }
}

void Text_drawCentered(SDL_Renderer* renderer, const char* str, int y) {
    Text_draw(renderer, str, (Vec2){WINDOW_WIDTH/2 - Text_getWidth(str)/2, y});
}

int Text_getWidth(const char* str) {
    int j = (SDL_GetTicks()/U_WOBBLE_SPEED)%2;

    int w = 0;
    for (const char* c = str; *c != '\0'; c += sizeof(char)) {
        w += Text_hitboxes[*c - ' '][j].w + T_SPACING;
    }
    return w - T_SPACING;
}

#include "Text.h"
#include "Util.h"
#include "main.h"

static SDL_Texture* font = NULL;
static int starts[93];
static int widths[93];

void Text_charMetrics(SDL_Surface* surface, int i, int* start, int* width) {
    // Manually override space character
    if (i == 0) {
        *start = 0;
        *width = T_SPACING;
        return;
    }

    *start = T_SIZE;
    *width = 0;
    for (int x = 0; x < T_SIZE; x++) {
        for (int y = 0; y < T_SIZE; y++) {
            Uint8 r, g, b, a;
            SDL_ReadSurfacePixel(surface, i*T_SIZE + x, y, &r, &g, &b, &a);
            if (a > 0) {
                *start = SDL_min(*start, x);
                *width = SDL_max(*width, x - *start);
            }
        }
    }
}

bool Text_init(SDL_Renderer* renderer) {
    SDL_Surface* fontSurface = Util_loadSurface("font.png");
    for (int i = 0; i < 93; i++) Text_charMetrics(fontSurface, i, &starts[i], &widths[i]);

    font = SDL_CreateTextureFromSurface(renderer, fontSurface);
    SDL_DestroySurface(fontSurface);

    return true;
}

void Text_deinit() {
    if (font != NULL) SDL_DestroyTexture(font);
}

void Text_draw(SDL_Renderer* renderer, const char* str, Vec2 pos) {
    SDL_FRect src_rect = {0, T_SIZE*((SDL_GetTicks()/U_WOBBLE_SPEED)%(font->h/T_SIZE)), T_SIZE, T_SIZE};
    SDL_FRect dst_rect = {pos.x, pos.y, T_SIZE, T_SIZE};

    for (const char* c = str; *c != '\0'; c += sizeof(char)) {
        int i = *c - ' ';
        // SDL_Log("c: %c, start: %d, width: %d", *c, starts[i], widths[i]);
        src_rect.x = i*T_SIZE + starts[i];
        src_rect.w = widths[i];
        dst_rect.w = widths[i];
        SDL_RenderTexture(renderer, font, &src_rect, &dst_rect);
        dst_rect.x += widths[i] + T_SPACING;
    }
}

void Text_drawCentered(SDL_Renderer* renderer, const char* str, int y) {
    Text_draw(renderer, str, (Vec2){WINDOW_WIDTH/2 - Text_getWidth(str)/2, y});
}

int Text_getWidth(const char* str) {
    int w = 0;
    for (const char* c = str; *c != '\0'; c += sizeof(char)) {
        w += widths[*c - ' '] + T_SPACING;
    }
    return w - T_SPACING;
}

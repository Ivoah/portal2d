#include "Text.h"
#include "Util.h"
#include "main.h"

#define T_NUM_CHARS ('}' - ' ')

static SDL_Texture* font = NULL;
static int starts[T_NUM_CHARS][2];
static int widths[T_NUM_CHARS][2];

void Text_metrics(SDL_Surface* surface, int i, int start[2], int width[2]) {
    // Manually override space character
    if (i == 0) {
        start[0] = start[1] = 0;
        width[0] = width[1] = T_SPACING;
        return;
    }

    start[0] = start[1] = T_SIZE;
    width[0] = width[1] = 0;
    for (int x = 0; x < T_SIZE; x++) {
        for (int y = 0; y < T_SIZE; y++) {
            for (int j = 0; j < 2; j++) {
                Uint8 a;
                SDL_ReadSurfacePixel(surface, i*T_SIZE + x, j*T_SIZE + y, NULL, NULL, NULL, &a);
                if (a > 0) {
                    start[j] = SDL_min(start[j], x);
                    width[j] = SDL_max(width[j], x - start[j] + 1);
                }
            }
        }
    }
}

bool Text_init(SDL_Renderer* renderer) {
    SDL_Surface* fontSurface = Util_loadSurface("font.png");
    for (int i = 0; i < T_NUM_CHARS; i++) Text_metrics(fontSurface, i, starts[i], widths[i]);

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
        // SDL_Log("c: %c, start: %d, width: %d", *c, starts[i][j], widths[i][j]);
        src_rect.x = i*T_SIZE + starts[i][j];
        src_rect.w = widths[i][j];
        dst_rect.w = widths[i][j];
        SDL_RenderTexture(renderer, font, &src_rect, &dst_rect);
        dst_rect.x += widths[i][j] + T_SPACING;
    }
}

void Text_drawCentered(SDL_Renderer* renderer, const char* str, int y) {
    Text_draw(renderer, str, (Vec2){WINDOW_WIDTH/2 - Text_getWidth(str)/2, y});
}

int Text_getWidth(const char* str) {
    int j = (SDL_GetTicks()/U_WOBBLE_SPEED)%2;

    int w = 0;
    for (const char* c = str; *c != '\0'; c += sizeof(char)) {
        w += widths[*c - ' '][j] + T_SPACING;
    }
    return w - T_SPACING;
}

#include "Text.h"
#include "Util.h"

const char* Text_texturePaths[] = {
    "level.png",
    "moves.png",
    "play.png"
};

static SDL_Texture* Text_textures[T_NUM_TX];
static SDL_Texture* Text_numbersTexture = NULL;

bool Text_init(SDL_Renderer* renderer) {
    for (int i = 0; i < T_NUM_TX; i++) {
        Text_textures[i] = Util_loadTexture(renderer, Text_texturePaths[i]);
        if (Text_textures[i] == NULL) return false;
    }

    Text_numbersTexture = Util_loadTexture(renderer, "numbers.png");
    if (Text_numbersTexture == NULL) return false;

    return true;
}

void Text_deinit() {
    for (int i = 0; i < T_NUM_TX; i++)
        if (Text_textures[i] != NULL) SDL_DestroyTexture(Text_textures[i]);
    if (Text_numbersTexture != NULL) SDL_DestroyTexture(Text_numbersTexture);
}

void Text_draw(SDL_Renderer* renderer, Text_TxId id, Vec2 pos) {
    SDL_Texture* tx = Text_textures[id];

    SDL_FRect src_rect = {0, T_TEXT_HEIGHT*((SDL_GetTicks()/T_WOBBLE_SPEED)%(tx->h/T_TEXT_HEIGHT)), tx->w, T_TEXT_HEIGHT};
    SDL_FRect dst_rect = {pos.x, pos.y, tx->w, T_TEXT_HEIGHT};

    SDL_RenderTexture(renderer, tx, &src_rect, &dst_rect);
}

void Text_drawNumber(SDL_Renderer* renderer, int num, int minDigits, int maxDigits, Vec2 pos) {
    SDL_FRect src_rect = {0, T_TEXT_HEIGHT*((SDL_GetTicks()/T_WOBBLE_SPEED)%(Text_numbersTexture->h/T_TEXT_HEIGHT)), T_TEXT_HEIGHT, T_TEXT_HEIGHT};
    SDL_FRect dst_rect = {0, pos.y, T_TEXT_HEIGHT, T_TEXT_HEIGHT};

    int numLen = SDL_log10(num) + 1;
    int digits = SDL_max(minDigits, maxDigits > 0 ? SDL_min(maxDigits, numLen) : numLen);
    for (int d = 0; d < digits; d++) {
        dst_rect.x = pos.x + (digits - (d + 1))*T_TEXT_HEIGHT;
        src_rect.x = (num/(int)SDL_pow(10, d)%10)*T_TEXT_HEIGHT;
        SDL_RenderTexture(renderer, Text_numbersTexture, &src_rect, &dst_rect);
    }
}

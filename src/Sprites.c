#include "Sprites.h"
#include "Util.h"

SDL_Texture* Sprites_texture;
Vec4 Sprites_hitboxes[S_NUM_TX] = {};

bool Sprites_init(SDL_Renderer* renderer, const char* path) {
    SDL_Surface* surface = Util_loadSurface(path);
    if (!surface) {
        SDL_Log("Couldn't load sprite surface: %s", SDL_GetError());
        return false;
    }
    for (int i = 1; i < S_NUM_TX; i++) Sprites_hitboxes[i] = Util_measureSprite(surface, (Vec4){i*S_SIZE, 0, S_SIZE, S_SIZE});

    Sprites_texture = SDL_CreateTextureFromSurface(renderer, surface);
    if (!Sprites_texture) {
        SDL_Log("Couldn't create sprite texture: %s", SDL_GetError());
        return false;
    }

    SDL_DestroySurface(surface);
    return true;
}

void Sprites_draw(SDL_Renderer* renderer, SpriteId tile, const Vec2* pos) {
    SDL_FRect src_rect = {tile*S_SIZE, 0, S_SIZE, S_SIZE};
    SDL_FRect dst_rect = {pos->x, pos->y, S_SIZE, S_SIZE};
    SDL_RenderTexture(renderer, Sprites_texture, &src_rect, &dst_rect);
}

Vec4 Sprites_hitbox(SpriteId tile) {
    return Sprites_hitboxes[tile];
}

void Sprites_drawPortal(SDL_Renderer* renderer, bool bluePortal, const Vec2* pos, double angle) {
    SDL_FRect src_rect = {(bluePortal ? S_BLUEPORTAL1 : S_ORANGEPORTAL1)*S_SIZE + (SDL_GetTicks()/100)%S_SIZE, 0, S_SIZE, S_SIZE};
    SDL_FRect dst_rect = {pos->x, pos->y, S_SIZE, S_SIZE};
    SDL_RenderTextureRotated(renderer, Sprites_texture, &src_rect, &dst_rect, angle, NULL, SDL_FLIP_NONE);
}

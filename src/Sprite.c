#include "Sprite.h"

void Sprite_draw(SDL_Renderer* renderer, SDL_Texture* tx, SpriteId tile, const Vec2* pos) {
    SDL_FRect src_rect = {tile*S_SIZE, 0, S_SIZE, S_SIZE};
    SDL_FRect dst_rect = {pos->x, pos->y, S_SIZE, S_SIZE};
    SDL_RenderTexture(renderer, tx, &src_rect, &dst_rect);
}

void Sprite_drawPortal(SDL_Renderer* renderer, SDL_Texture* tx, bool bluePortal, const Vec2* pos, double angle) {
    SDL_FRect src_rect = {(bluePortal ? S_BLUEPORTAL1 : S_ORANGEPORTAL1)*S_SIZE + (SDL_GetTicks()/100)%S_SIZE, 0, S_SIZE, S_SIZE};
    SDL_FRect dst_rect = {pos->x, pos->y, S_SIZE, S_SIZE};
    SDL_RenderTextureRotated(renderer, tx, &src_rect, &dst_rect, angle, NULL, SDL_FLIP_NONE);
}

#include "Entity.h"
#include "Level.h"

#define E_GRAVITY 1

void Entity_update(Entity* e, Level* level, int dt) {
    Vec2 newPos = Vec2_add(&e->pos, &e->velocity);
    int w2 = e->hitbox.w/2;
    int h2 = e->hitbox.h/2;
    if (
        Level_getTile(level, (Vec2){newPos.x - w2, newPos.y - h2}) != S_AIR ||
        Level_getTile(level, (Vec2){newPos.x + w2, newPos.y - h2}) != S_AIR ||
        Level_getTile(level, (Vec2){newPos.x + w2, newPos.y + h2}) != S_AIR ||
        Level_getTile(level, (Vec2){newPos.x - w2, newPos.y + h2}) != S_AIR
    ) return;
    e->pos = newPos;
}

void Entity_draw(SDL_Renderer* renderer, Entity* e) {
    SDL_FRect src_rect = {e->sprite*S_SIZE + e->hitbox.x, e->hitbox.y, e->hitbox.w, e->hitbox.h};
    SDL_FRect dst_rect = {e->pos.x - e->hitbox.w/2, e->pos.y - e->hitbox.h/2, e->hitbox.w, e->hitbox.h};
    SDL_RenderTexture(renderer, Sprites_texture, &src_rect, &dst_rect);
    SDL_SetRenderDrawColor(renderer, 0, 0, 255, SDL_ALPHA_OPAQUE);
    SDL_RenderLines(renderer, (SDL_FPoint[]){
        (SDL_FPoint){e->pos.x - e->hitbox.w/2, e->pos.y - e->hitbox.h/2},
        (SDL_FPoint){e->pos.x + e->hitbox.w/2, e->pos.y - e->hitbox.h/2},
        (SDL_FPoint){e->pos.x + e->hitbox.w/2, e->pos.y + e->hitbox.h/2},
        (SDL_FPoint){e->pos.x - e->hitbox.w/2, e->pos.y + e->hitbox.h/2},
        (SDL_FPoint){e->pos.x - e->hitbox.w/2, e->pos.y - e->hitbox.h/2},
    }, 5);
}

Entity Entity_create(SpriteId id, Vec2 pos) {
    return (Entity){
        .sprite = id,
        .pos = pos,
        .velocity = (Vec2){0, 0},
        .hitbox = Sprites_hitbox(id)
    };
}

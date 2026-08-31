#include "Entity.h"

void Entity_update(Entity* e, int dt) {
    // e->pos.y -= 1;
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
        .hitbox = Sprites_hitbox(id)
    };
}

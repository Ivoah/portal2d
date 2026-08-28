#include "Entity.h"

void Entity_update(Entity* e, int dt) {
    e->pos.y -= 1;
}

void Entity_draw(SDL_Renderer* renderer, SDL_Texture* tx, Entity* e) {
    SDL_FRect src_rect = {e->sprite*S_SIZE, 0, S_SIZE, S_SIZE};
    SDL_FRect dst_rect = {e->pos.x, e->pos.y, S_SIZE, S_SIZE};
    SDL_RenderTexture(renderer, tx, &src_rect, &dst_rect);
}

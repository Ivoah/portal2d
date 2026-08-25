#include "Util.h"

SDL_Surface* Util_loadSurface(const char* path) {
    char* pngPath = NULL;
    SDL_asprintf(&pngPath, "%ssprites/%s", SDL_GetBasePath(), path);
    SDL_Surface* surface = SDL_LoadPNG(pngPath);
    SDL_free(pngPath);
    return surface;
}

SDL_Texture* Util_loadTexture(SDL_Renderer* renderer, const char* path) {
    SDL_Surface* surface = Util_loadSurface(path);
    if (!surface) {
        SDL_Log("Couldn't load png: %s", SDL_GetError());
        return NULL;
    }
    
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    if (!texture) {
        SDL_Log("Couldn't create static texture: %s", SDL_GetError());
        return NULL;
    }

    SDL_DestroySurface(surface);

    return texture;
}

void Util_drawWobbly(SDL_Renderer* renderer, SDL_Texture* texture, Vec2 pos) {
    int height = texture->h/2;

    SDL_FRect src_rect = {0, height*((SDL_GetTicks()/U_WOBBLE_SPEED)%(texture->h/height)), texture->w, height};
    SDL_FRect dst_rect = {pos.x, pos.y, texture->w, height};

    SDL_RenderTexture(renderer, texture, &src_rect, &dst_rect);
}

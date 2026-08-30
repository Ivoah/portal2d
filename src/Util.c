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

Vec4 Util_measureSprite(SDL_Surface* surface, Vec4 bounds) {
    int minX = bounds.w, maxX = 0, minY = bounds.h, maxY = 0;
    for (int x = 0; x < bounds.w; x++) {
        for (int y = 0; y < bounds.h; y++) {
            Uint8 a;
            SDL_ReadSurfacePixel(surface, bounds.x + x, bounds.y + y, NULL, NULL, NULL, &a);
            if (a > 0) {
                minX = SDL_min(minX, x);
                maxX = SDL_max(maxX, x);
                minY = SDL_min(minY, y);
                maxY = SDL_max(maxY, y);
            }
        }
    }
    return (Vec4){minX, minY, maxX - minX + 1, maxY - minY + 1};
}

void Util_drawWobbly(SDL_Renderer* renderer, SDL_Texture* texture, Vec2 pos) {
    int height = texture->h/2;

    SDL_FRect src_rect = {0, height*((SDL_GetTicks()/U_WOBBLE_SPEED)%2), texture->w, height};
    SDL_FRect dst_rect = {pos.x, pos.y, texture->w, height};

    SDL_RenderTexture(renderer, texture, &src_rect, &dst_rect);
}

void Util_wrap(int* p, int size) {
    if (*p < 0) *p = size - 1;
    else if (*p >= size) *p = 0;
}

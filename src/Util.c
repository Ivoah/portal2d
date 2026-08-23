#include "Util.h"

SDL_Texture* Util_loadTexture(SDL_Renderer* renderer, const char* path) {
    char* pngPath = NULL;
    SDL_Surface* surface = NULL;
    SDL_Texture* texture = NULL;

    SDL_asprintf(&pngPath, "%ssprites/%s", SDL_GetBasePath(), path);
    surface = SDL_LoadPNG(pngPath);
    SDL_free(pngPath);

    if (!surface) {
        SDL_Log("Couldn't load png: %s", SDL_GetError());
        return NULL;
    }
    
    texture = SDL_CreateTextureFromSurface(renderer, surface);
    if (!texture) {
        SDL_Log("Couldn't create static texture: %s", SDL_GetError());
        return NULL;
    }

    SDL_DestroySurface(surface);

    return texture;
}

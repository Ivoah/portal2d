#include "Level.h"

#include "Sprites.h"

Level* Level_load(int num) {
    char* levelPath = NULL;

    SDL_asprintf(&levelPath, "%slevels/%d.txt", SDL_GetBasePath(), num);
    char* mapData = SDL_LoadFile(levelPath, NULL);
    SDL_free(levelPath);
    if (mapData == NULL) {
        SDL_Log("Could not load map: %s", SDL_GetError());
        return NULL;
    }
    Level* newLevel = SDL_calloc(1, sizeof(Level));

    int x = 0, y = 0;
    for (int i = 0; mapData[i] != 0; i++) {
        if ((x >= L_MAX_WIDTH || y >= L_MAX_HEIGHT) && mapData[i] != '\n') {
            SDL_Log("Level too large, max width/height is %dx%d, got %c at %dx%d", L_MAX_WIDTH, L_MAX_HEIGHT, mapData[i], x, y);
                Level_free(newLevel);
                SDL_free(mapData);
                return NULL;
        }
        switch (mapData[i]) {
            case '#':
                newLevel->tiles[y][x] = S_PWALL;
                break;
            case '@':
                newLevel->tiles[y][x] = S_NPWALL;
                break;
            case 'p':
                newLevel->entities[newLevel->nEntities] = Entity_create(S_BENDY, (Vec2){x*S_SIZE + S_SIZE/2, y*S_SIZE + S_SIZE/2});
                newLevel->nEntities++;
                break;
            case '\n':
                newLevel->width = SDL_max(newLevel->width, x);
                x = -1;
                y++;
                break;
            case ' ':
                break;
            default:
                SDL_Log("Unknown character in map file at (%d, %d): %c", x, y, mapData[i]);
                Level_free(newLevel);
                SDL_free(mapData);
                return NULL;
        }
        x += 1;
    }
    newLevel->width = SDL_max(newLevel->width, x);
    newLevel->height = x == 0 ? y : y + 1;

    SDL_free(mapData);

    return newLevel;
}

void Level_free(Level* level) {
    SDL_free(level);
}

void Level_update(Level* level, int delta) {
    for (int i = 0; i < level->nEntities; i++) {
        Entity_update(&level->entities[i], delta);
    }
}

void Level_draw(SDL_Renderer* renderer, Level* level) {
    // const Vec2 offset = {WINDOW_WIDTH/2 - level->width*L_TILE_SIZE/2, WINDOW_HEIGHT/2 - level->height*L_TILE_SIZE/2};
    
    // Draw map
    Vec2 dst = {0, 0};
    for (int i = 0; i < level->height; i++) {
        for (int j = 0; j < level->width; j++) {
            int tile = level->tiles[i][j];
            if (tile > 0) {
                dst.x = j*S_SIZE;
                dst.y = i*S_SIZE;
                Sprites_draw(renderer, tile, &dst);
            }
        }
    }

    // Draw entities
    for (int i = 0; i < level->nEntities; i++) {
        Entity_draw(renderer, &level->entities[i]);
    }

    // // Draw portals
    // if (level->state->bluePortal.exists) {
    //     dst.x = level->state->bluePortal.pos.x*L_TILE_SIZE + offset.x;
    //     dst.y = level->state->bluePortal.pos.y*L_TILE_SIZE + offset.y;
    //     Level_drawPortal(renderer, tx, true, &dst, level->state->bluePortal.r*90);
    // }
    // if (level->state->orangePortal.exists) {
    //     dst.x = level->state->orangePortal.pos.x*L_TILE_SIZE + offset.x;
    //     dst.y = level->state->orangePortal.pos.y*L_TILE_SIZE + offset.y;
    //     Level_drawPortal(renderer, tx, false, &dst, level->state->orangePortal.r*90);
    // }

    // // Draw player
    // dst.x = level->state->playerLocation.x*L_TILE_SIZE + offset.x;
    // dst.y = level->state->playerLocation.y*L_TILE_SIZE + offset.y;
    // Level_drawTile(renderer, tx, L_BENDY, &dst);
}

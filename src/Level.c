#include "Level.h"
#include "Util.h"
#include "Text.h"

const char* Level_texturePaths[] = {
    NULL,
#define L_BUTTON 1
    "button.png",
#define L_CUBE   2
    "cube.png",
#define L_PLAYER 3
    "bendy.png",
#define L_FLOOR  4
    "floor.png",
#define L_PWALL  5
    "pwall.png",
#define L_NPWALL 6
    "npwall.png",
#define L_ORANGEPORTAL 7
    "orangePortal.png",
#define L_BLUEPORTAL 8
    "bluePortal.png",
#define L_LEVEL 9
    "level.png",
#define L_MOVES 10
    "moves.png"
};
#define L_NUM_TX    11

static SDL_Texture* Level_textures[L_NUM_TX];

bool Level_init(SDL_Renderer* renderer) {
    // Skip first entry in texture_paths
    for (int i = 1; i < L_NUM_TX; i++) {
        Level_textures[i] = Util_loadTexture(renderer, Level_texturePaths[i]);
        if (Level_textures[i] == NULL) return false;
    }

    return true;
}

void Level_deinit() {
    for (int i = 0; i < L_NUM_TX; i++) if (Level_textures[i] != NULL) SDL_DestroyTexture(Level_textures[i]);
}

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
    newLevel->state = SDL_calloc(1, sizeof(LevelState));
    newLevel->levelNum = num;

    int x = 0, y = 0;
    for (int i = 0; mapData[i] != 0; i++) {
        if (x >= L_MAX_WIDTH || y >= L_MAX_HEIGHT) {
            SDL_Log("Level too large, max width/height is %dx%d", L_MAX_WIDTH, L_MAX_HEIGHT);
        }
        switch (mapData[i]) {
            case '?':
                newLevel->tiles[y][x] = L_BUTTON;
                break;
            case 'c':
                if (newLevel->numCubes >= L_MAX_CUBES) {
                    SDL_Log("too many cubes!");
                    Level_free(newLevel);
                    SDL_free(mapData);
                    return NULL;
                }
                newLevel->tiles[y][x] = L_FLOOR;
                newLevel->state->cubes[newLevel->numCubes].x = x;
                newLevel->state->cubes[newLevel->numCubes].y = y;
                newLevel->numCubes += 1;
                break;
            case '>':
                newLevel->tiles[y][x] = L_FLOOR;
                newLevel->state->playerLocation.x = x;
                newLevel->state->playerLocation.y = y;
                break;
            case '.':
                newLevel->tiles[y][x] = L_FLOOR;
                break;
            case '#':
                newLevel->tiles[y][x] = L_PWALL;
                break;
            case '@':
                newLevel->tiles[y][x] = L_NPWALL;
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

void Level_freeState(LevelState* state) {
    if (state->lastState != NULL) Level_freeState(state->lastState);
    SDL_free(state);
}

void Level_free(Level* level) {
    SDL_free(level->state);
    SDL_free(level);
}

void Level_draw(SDL_Renderer* renderer, Level* level, Vec2 offset) {
    SDL_FRect src_rect = {0, 0, L_TILE_SIZE, L_TILE_SIZE};
    SDL_FRect dst_rect = {0, 0, L_TILE_SIZE, L_TILE_SIZE};

    // Draw map
    for (int i = 0; i < level->height; i++) {
        for (int j = 0; j < level->width; j++) {
            int tile = level->tiles[i][j];
            if (tile > 0) {
                dst_rect.x = j*L_TILE_SIZE + offset.x;
                dst_rect.y = i*L_TILE_SIZE + offset.y;
                SDL_RenderTexture(renderer, Level_textures[tile], NULL, &dst_rect);
            }
        }
    }

    // Draw cubes
    for (int i = 0; i < level->numCubes; i++) {
        dst_rect.x = level->state->cubes[i].x*L_TILE_SIZE + offset.x;
        dst_rect.y = level->state->cubes[i].y*L_TILE_SIZE + offset.y;
        SDL_RenderTexture(renderer, Level_textures[L_CUBE], NULL, &dst_rect);
    }

    // Draw portals
    src_rect.x = (SDL_GetTicks()/100)%L_TILE_SIZE;
    if (level->state->bluePortal.exists) {
        dst_rect.x = level->state->bluePortal.pos.x*L_TILE_SIZE + offset.x;
        dst_rect.y = level->state->bluePortal.pos.y*L_TILE_SIZE + offset.y;
        SDL_RenderTextureRotated(renderer, Level_textures[L_BLUEPORTAL], &src_rect, &dst_rect, level->state->bluePortal.r*90, NULL, SDL_FLIP_NONE);
    }
    if (level->state->orangePortal.exists) {
        dst_rect.x = level->state->orangePortal.pos.x*L_TILE_SIZE + offset.x;
        dst_rect.y = level->state->orangePortal.pos.y*L_TILE_SIZE + offset.y;
        SDL_RenderTextureRotated(renderer, Level_textures[L_ORANGEPORTAL], &src_rect, &dst_rect, level->state->orangePortal.r*90, NULL, SDL_FLIP_NONE);
    }

    // Draw player
    dst_rect.x = level->state->playerLocation.x*L_TILE_SIZE + offset.x;
    dst_rect.y = level->state->playerLocation.y*L_TILE_SIZE + offset.y;
    SDL_RenderTexture(renderer, Level_textures[L_PLAYER], NULL, &dst_rect);

    // Draw level number
    // src_rect = (SDL_FRect){0, L_TILE_SIZE*((SDL_GetTicks()/500)%(Level_textures[L_LEVEL]->h/L_TILE_SIZE)), Level_textures[L_LEVEL]->w, L_TILE_SIZE};
    // dst_rect.x = 0;
    // dst_rect.y = 0;
    // dst_rect.w = Level_textures[L_LEVEL]->w;
    // SDL_RenderTexture(renderer, Level_textures[L_LEVEL], &src_rect, &dst_rect);
    Text_draw(renderer, T_LEVEL, (Vec2){0, 0});
    Text_drawNumber(renderer, level->levelNum, 2, -1, (Vec2){Level_textures[L_LEVEL]->w, 0});

    // Draw move counter
    // src_rect = (SDL_FRect){0, L_TILE_SIZE*((SDL_GetTicks()/500)%(Level_textures[L_MOVES]->h/L_TILE_SIZE)), Level_textures[L_MOVES]->w, L_TILE_SIZE};
    // dst_rect.x = 0;
    // dst_rect.y = L_TILE_SIZE;
    // dst_rect.w = Level_textures[L_MOVES]->w;
    // SDL_RenderTexture(renderer, Level_textures[L_MOVES], &src_rect, &dst_rect);
    Text_draw(renderer, T_MOVES, (Vec2){0, T_TEXT_HEIGHT});
    Text_drawNumber(renderer, level->state->moves, 2, -1, (Vec2){Level_textures[L_MOVES]->w, L_TILE_SIZE});
}

void Level_newState(Level* level) {
    LevelState* newState = SDL_malloc(sizeof(LevelState));
    SDL_memcpy(newState, level->state, sizeof(LevelState));
    newState->lastState = level->state;
    level->state = newState;
    level->state->moves++;
}

int Level_isCube(Level* level, Vec2 pos) {
    for (int i = 0; i < level->numCubes; i++) {
        if (level->state->cubes[i].x == pos.x && level->state->cubes[i].y == pos.y) {
            return i;
        }
    }

    return -1;
}

bool Level_isPassable(Level* level, Vec2 pos) {
    if (Level_isCube(level, pos) > -1) return false;

    switch (level->tiles[pos.y][pos.x]) {
        case L_PWALL:
        case L_NPWALL:
            return false;
        default:
            return true;
    }
}

void Level_moveThroughPortals(Level* level, Vec2* pos, Vec2* dir) {
    *pos = Vec2_add(pos, dir);
    if (level->state->bluePortal.exists && level->state->orangePortal.exists) {
        if (Vec2_equal(pos, &level->state->bluePortal.pos) && Vec2_toR(dir) == level->state->bluePortal.r) {
            *dir = Vec2_fromR(level->state->orangePortal.r);
            *dir = Vec2_180(dir);
            *pos = Vec2_add(&level->state->orangePortal.pos, dir);
        } else if (Vec2_equal(pos, &level->state->orangePortal.pos) && Vec2_toR(dir) == level->state->orangePortal.r) {
            *dir = Vec2_fromR(level->state->bluePortal.r);
            *dir = Vec2_180(dir);
            *pos = Vec2_add(&level->state->bluePortal.pos, dir);
        }
    }
}

bool Level_canMove(Level* level, Vec2 pos, Vec2 dir) {
    Level_moveThroughPortals(level, &pos, &dir);
    if (Level_isCube(level, pos) > -1) return Level_canMove(level, pos, dir);

    switch (level->tiles[pos.y][pos.x]) {
        case L_PWALL:
        case L_NPWALL:
            return false;
        default:
            return true;
    }
}

void Level_moveEntity(Level* level, Vec2* ent, Vec2 dir) {
    Vec2 newPos = *ent;
    Level_moveThroughPortals(level, &newPos, &dir);
    int nextCubeId = Level_isCube(level, newPos);
    if (nextCubeId > -1) Level_moveEntity(level, &level->state->cubes[nextCubeId], dir);
    *ent = newPos;
}

void Level_move(Level* level, Vec2 dir) {
    if (Level_canMove(level, level->state->playerLocation, dir)) {
        Level_newState(level);
        Level_moveEntity(level, &level->state->playerLocation, dir);
    }
}

void Level_shoot(Level* level, Vec2 dir) {
    Vec2 pos = level->state->playerLocation;
    while (Level_isPassable(level, pos)) pos = Vec2_add(&pos, &dir);
    if (level->tiles[pos.y][pos.x] == L_PWALL) {
        Level_newState(level);
        Portal* newPortal = (level->state->lastShotBlue) ? &level->state->orangePortal : &level->state->bluePortal;
        level->state->lastShotBlue = !level->state->lastShotBlue;
        newPortal->exists = true;
        newPortal->pos = pos;
        newPortal->r = Vec2_toR(&dir);
    }
}

void Level_undo(Level* level, int steps) {
    if (level->state->lastState != NULL && steps != 0) {
        LevelState* tmp = level->state;
        level->state = level->state->lastState;
        SDL_free(tmp);
        Level_undo(level, steps - 1);
    }
}

bool Level_isWon(Level* level) {
    for (int i = 0; i < level->height; i++) {
        for (int j = 0; j < level->width; j++) {
            if (level->tiles[i][j] == L_BUTTON && Level_isCube(level, (Vec2){j, i}) == -1) return false;
        }
    }

    return true;
}

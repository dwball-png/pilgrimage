/*
  Daniel Ball
  gameLevel.cpp
  CPSC 4160
*/

#include "gameLevel.h"

GameLevel::GameLevel() {}
GameLevel::~GameLevel() {}

void GameLevel::level_init(SDL_Renderer* r) {
    lvl_renderer = r;
    // create the layout for the map
    for (int i = -4; i < 24; i++) {
        if (!(i > 7 && i < 12)) {
            SDL_Rect* newRect = new SDL_Rect();
            newRect->x = i*32;
            newRect->y = 416;
            newRect->w = 32;
            newRect->h = 32;
            tile_list.push_back(newRect);
        }
    }
    SDL_Rect* rect1 = new SDL_Rect();
    rect1->x = 224;
    rect1->y = 448;
    rect1->w = 32;
    rect1->h = 32;
    tile_list.push_back(rect1);
    SDL_Rect* rect2 = new SDL_Rect();
    rect2->x = 384;
    rect2->y = 448;
    rect2->w = 32;
    rect2->h = 32;
    tile_list.push_back(rect2);

    // assign tiles
    for (int i = 0; i < 15; i++) {
        for (int j = 0; j < 20; j++) {
            if (i == 13 && (!(j > 7 && j < 12))) {
                GameLevel::tilemap[i][j] = 1;
            }
            else {
                GameLevel::tilemap[i][j] = 0;
            }
        }
    }
    GameLevel::tilemap[13][7] = 2;
    GameLevel::tilemap[14][7] = 3;
    GameLevel::tilemap[13][12] = 5;
    GameLevel::tilemap[14][12] = 4;

    SDL_Surface* temp = IMG_Load("./assets/tile_land.png");
	tile_texture = SDL_CreateTextureFromSurface(lvl_renderer, temp);
	SDL_FreeSurface(temp);
    src.x = 0;
    src.y = 0;
    src.w = 16;
    src.h = 16;
}

void GameLevel::level_render() {

    // render tiles
    for (SDL_Rect* tile : tile_list) {
        int off = get_tile(tile->x, tile->y);
        if (off != 0) {
            src.x = (off-1)*16;
            SDL_RenderCopy(lvl_renderer, tile_texture, &src, tile);
        }
    }
}

int GameLevel::get_tile(int x, int y) {
    return tilemap[y/32][x/32];
}

// list of collision tiles
std::vector<SDL_Rect*> GameLevel::get_tiles() {
    return tile_list;
}
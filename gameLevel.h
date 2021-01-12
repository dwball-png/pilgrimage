/*
  Daniel Ball
  gameLevel.h
  CPSC 4160
*/

#pragma once
#include "sprite.h"
#include <SDL2/SDL.h> 
#include <SDL2/SDL_image.h>
#include <vector>

class GameLevel {
    public:
        GameLevel();
        ~GameLevel();

        void level_init(SDL_Renderer* r);
        void level_render();

        int get_tile(int x, int y);
        std::vector<SDL_Rect*> get_tiles();

    private:
        SDL_Renderer* lvl_renderer;
        SDL_Texture* tile_texture;
        int tilemap[480/32][640/32];
        SDL_Rect src;
        
        std::vector<SDL_Rect*> tile_list;
};
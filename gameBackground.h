/*
  Daniel Ball
  gameBackground.h
  CPSC 4160
*/

#pragma once
#include "sprite.h"
#include <SDL2/SDL.h> 
#include <SDL2/SDL_image.h>

class GameBackground {
    public:
        GameBackground(char* path, SDL_Renderer* r, int srcx, int srcy, int srcw, int srch,
	int desx, int desy, int desw, int desh, int vx);
        ~GameBackground();
        void bck_render();
        void bck_quit();
    private:
        SDL_Renderer* bck_renderer;
        SDL_Rect bck_rect;
        SDL_Rect spr_rect;
        SDL_Texture* spr_texture;
        Sprite* bck_sprite;
        int x_vel;
};
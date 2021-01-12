/*
  Daniel Ball
  objectText.h
  CPSC 4160
*/

#pragma once
#include <SDL2/SDL.h> 
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

class ObjectText {
    public: 
        ObjectText(SDL_Renderer* r, int x, int y, int size);
        ~ObjectText();
        void obj_init(const char* ttf_file, SDL_Color col, int size);
        void obj_update(const char* str);
        void obj_render();
    private:
        SDL_Renderer* obj_renderer;
        SDL_Texture* txt_texture;
        SDL_Rect obj_rect;
        int x_pos;
        int y_pos;
        TTF_Font* txt_fnt;
        SDL_Color txt_col;
};
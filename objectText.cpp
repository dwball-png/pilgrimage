/*
  Daniel Ball
  objectText.cpp
  CPSC 4160
*/

#include "objectText.h"

ObjectText::ObjectText(SDL_Renderer* r, int x, int y, int size) {
    obj_renderer = r;
    x_pos = x;
    y_pos = y;
    obj_rect.x = x_pos;
    obj_rect.y = y_pos;
    SDL_Color c;
    c.r = 235;
    c.g = 211;
    c.b = 32;
    obj_init("./assets/fnt_texturina.ttf", c, size);
}

ObjectText::~ObjectText() {
    
}

void ObjectText::obj_init(const char* ttf_file, SDL_Color col, int size) {
    txt_fnt = TTF_OpenFont(ttf_file, size);
    //std::cout << TTF_GetError();
    txt_col = col;
}

void ObjectText::obj_update(const char* str) {
    SDL_Surface *txt_surface;
    if (!(txt_surface=TTF_RenderText_Solid(txt_fnt, str, txt_col))) {
        std::cout << "font error\n";
    }
    else {
        txt_texture = SDL_CreateTextureFromSurface(obj_renderer, txt_surface);
        SDL_FreeSurface(txt_surface);
        int w, h;
        SDL_QueryTexture(txt_texture, NULL, NULL, &w, &h);
        // scale 2x to match pixels, center
        obj_rect.w = w*2;
        obj_rect.h = h*2;
        obj_rect.x -= obj_rect.w/2;
        obj_rect.y -= obj_rect.h/2;
    }
}

void ObjectText::obj_render() {
    SDL_RenderCopy(obj_renderer, txt_texture, NULL, &obj_rect);
}
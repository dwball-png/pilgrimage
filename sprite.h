/*
  Daniel Ball
  sprite.h
  CPSC 4160
*/

#pragma once
#include <SDL2/SDL.h> 
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_timer.h>

class Sprite {
    public:
        Sprite();
        ~Sprite();

        void spr_init(const char* path, SDL_Renderer* ren, int fc, int fd, int w, int h);
        SDL_Rect spr_update();
        void spr_render(SDL_Renderer* r);
        void spr_quit();

        SDL_Texture* get_texture();
        int get_frame();
        int get_frame_count();

    private:
        SDL_Texture* spr_texture;
        SDL_Rect spr_rect;

        int frame_count;
        int frame_duration;
        int previous_frame = 0;
        int this_frame;
        SDL_Renderer* game_renderer;
};
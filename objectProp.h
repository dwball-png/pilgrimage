/*
  Daniel Ball
  objectProp.h
  CPSC 4160
*/

class GameEngine;
#pragma once
#include "gameEngine.h"
#include "sprite.h"
#include <SDL2/SDL.h> 
#include <SDL2/SDL_image.h>

class ObjectProp {
    public:
        ObjectProp();
        ObjectProp(SDL_Renderer* r, int x, int y, int w, int h);
        ~ObjectProp();
        virtual void obj_init(int x, int y, int w, int h);
        virtual void obj_update(GameEngine* e);
        virtual void obj_render();

        SDL_Rect* get_msk();

        SDL_Renderer* get_renderer();
        Sprite* get_sprite();

        static double tween(double x1, double x2, double t) {
		return x1 * (1-t) + (x2 * t);
		}
        void cleanup();
        bool garbage = false;
    protected:
        SDL_Renderer* obj_renderer;
        SDL_Rect spr_rect;
        SDL_Texture* spr_texture;
        Sprite* obj_sprite;

        SDL_RendererFlip spr_facing;

        SDL_Rect obj_rect;
        SDL_Rect msk_rect;
        int msk_x_offset;
        int msk_y_offset;

        double x_pos;
        double y_pos;
        double x_vel;
        double y_vel;
};

class ObjectCandy : public ObjectProp {
    public:
        ObjectCandy();
        ObjectCandy(SDL_Renderer* r, int x, int y);
        ~ObjectCandy();
        void obj_init();
        void obj_update(GameEngine* e);
        void obj_render();
        void collect(int sect);
    private:
        int base_y;
        int timer;
        int count = 0;
        SDL_Rect sector_1;
        SDL_Rect sector_2;
        SDL_Rect sector_3;
        SDL_Rect ui_rect;
        Sprite* ui_sprite;
        SDL_Texture* ui_texture;
        SDL_Rect score_rect;
        SDL_Rect score_src_rect;
        Sprite* score_sprite = new Sprite();
        SDL_Texture* score_texture;
        int init_score_x = 576;
        int off_ones = 0;
        int off_tens = 0;
};
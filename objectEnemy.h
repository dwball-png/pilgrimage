/*
  Daniel Ball
  objectEnemy.h
  CPSC 4160
*/

class GameEngine;
#pragma once
#include "gameEngine.h"
#include "sprite.h"
#include <SDL2/SDL.h> 
#include <SDL2/SDL_image.h>

class ObjectEnemy {
    public:
        ObjectEnemy(SDL_Renderer* r, int x, int y, int w, int h);
        ~ObjectEnemy();
        virtual void obj_init(int x, int y, int w, int h);
        virtual void obj_update(GameEngine* e);
        virtual void obj_render();
        void obj_quit();

        int get_x();
        int get_y();
        void set_x(int x);
        void set_y(int y);
        int get_x_vel();
        int get_y_vel();
        void set_x_vel(int xv);
        void set_y_vel(int yv);
        int get_state();
        void set_state(int s);
        SDL_Rect* get_msk();

        SDL_Renderer* get_renderer();
        Sprite* get_sprite();

        static double tween(double x1, double x2, double t) {
		return x1 * (1-t) + (x2 * t);
		}
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
        int x_prev;
        int y_prev;
        double x_vel;
        double y_vel;
        int state;
        int state_previous;
};

class ObjectWitch : public ObjectEnemy {
    public:
        ObjectWitch(SDL_Renderer* r, int x, int y, int w, int h, int xoff, int yoff);
        ~ObjectWitch();
        void obj_init();
        void obj_update(GameEngine* e);
        void obj_render();
        void obj_quit();

        static const int STATE_IDLE = 0;
        static const int STATE_GLIDE = 1;
        static const int STATE_SWOOP = 2;
        static const int STATE_CAST = 3;
        static const int STATE_EXIT = 4;
    private:
        int dir = 1;
        int base_y;
        int base_vel;
        int timer = 0;
        static int set_timer() {
            return (rand() % 50) + 150;
        }
};

class ObjectFireball : public ObjectEnemy {
    public:
        ObjectFireball(SDL_Renderer* r, int x, int y);
        ~ObjectFireball();
        void obj_init(int tx, int ty);
        void obj_update(GameEngine* e);
        void obj_render();
        void collect();
    private:
        int base_vel = 3;
};
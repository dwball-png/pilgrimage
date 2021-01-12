/*
  Daniel Ball
  objectPlayer.h
  CPSC 4160
*/

class GameEngine;
#pragma once
#include "gameEngine.h"
#include "sprite.h"
#include "gameLevel.h"
#include <SDL2/SDL.h> 
#include <SDL2/SDL_image.h>

class ObjectPlayer {
	public:
		ObjectPlayer(SDL_Renderer* r, int x, int y, int w, int h);
		~ObjectPlayer();
		void obj_init(int x, int y, int w, int h);
		void obj_update(GameLevel* l, GameEngine* e);
		void obj_render();
		void obj_quit();

		int get_x();
		int get_y();
		void set_x(int x); 
		void set_y(int y);
		double get_x_vel();
		double get_y_vel();
		void set_x_vel(double xv); 
		void set_y_vel(double yv);
		int get_state();
		void set_state(int s);
		void reset();

		static const int MAX_VEL = 3;
		static const int JUMP_VEL = 12;

		static const int STATE_IDLE = 0;
		static const int STATE_WALK = 1;
		static const int STATE_JUMP = 2;
		static const int STATE_DEATH = 3;
		static const int STATE_REBIRTH = 4;
		static const int STATE_BEGIN = 5;

		static double tween_function(double x1, double x2, double t) {
		return x1 * (1-t) + (x2 * t);
		}

		SDL_Renderer* get_renderer();
		Sprite* get_sprite();

	private:
		SDL_Renderer* obj_renderer;
		SDL_Rect spr_rect;
		SDL_Texture* spr_texture;
		Sprite* obj_sprite;

		SDL_RendererFlip spr_facing;

		SDL_Rect obj_rect;
		SDL_Rect msk_rect;
		int msk_x_offset = 48;
		int msk_y_offset = 32;

		int x_pos;
		int y_pos;
		int x_prev;
		int y_prev;
		double x_vel;
		double y_vel;
		int state;
		int state_previous;
		int sector = 0;
		short dead = 0;
		int dead_off = 0;
};
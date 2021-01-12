/*
  Daniel Ball
  objectPlayer.cpp
  CPSC 4160
*/

#include "objectPlayer.h"
#include "gameCollision.h"
#include "objectProp.h"
#include <SDL2/SDL_timer.h>
#include <typeinfo>

// creates player object
ObjectPlayer::ObjectPlayer(SDL_Renderer* r, int x, int y, int w, int h) {
	obj_renderer = r;
	obj_sprite = new Sprite();
	spr_facing = SDL_FLIP_NONE;
	obj_init(x, y, w, h);
}

ObjectPlayer::~ObjectPlayer() {}

// initializes player object 
void ObjectPlayer::obj_init(int x, int y, int w, int h) {
	obj_rect.x = x;
	obj_rect.y = y;
	obj_rect.w = w;
	obj_rect.h = h;
	// msk_rect hitbox
	msk_rect.x = x+msk_x_offset;
	msk_rect.y = y+msk_y_offset;
	msk_rect.w = 32;
	msk_rect.h = h-msk_y_offset-6;
	x_pos = x;
	y_pos = y;
	x_prev = x_pos;
	y_prev = y_pos;
	x_vel = 0;
	y_vel = 0;
	state = 0;
	state_previous = 0;
}

// updates player object
void ObjectPlayer::obj_update(GameLevel* l, GameEngine* e) {
	// update sprites
	spr_rect = obj_sprite->spr_update();
	spr_texture = obj_sprite->get_texture();
	// static at beginning
	if (state == STATE_BEGIN) {

	}
	else {
	// death animation
	if (state == STATE_DEATH) {
		y_pos = tween_function(y_pos, dead_off, 0.1);
		if (y_pos - dead_off <= 0.001) {
			// stop rendering
			dead = 2;
			state = STATE_REBIRTH;
		}
	}
	else
	// waiting for restart
	if (state == STATE_REBIRTH) {

	}
	else
	{
		// update mirroring
		if (x_vel > 0) {
			spr_facing = SDL_FLIP_NONE;
		}
		else if (x_vel < 0) {
			spr_facing = SDL_FLIP_HORIZONTAL;
		}
		// update y velocity
		if (state == STATE_JUMP) {
			y_vel += 0.5;
		}
		// collision handling
		// horizontal collision
		std::vector<SDL_Rect*> collidables = l->get_tiles();
		for (SDL_Rect* s : collidables) {
			// check for collision on next frame
			if (Collision::placeMeeting(&msk_rect, s, x_vel, 0)) {
				int sign = 1;
				if (x_vel < 0) {sign = -1;}
				// move to point of collision
				while (!Collision::placeMeeting(&msk_rect, s, sign, 0)) {
					x_pos += sign;
					msk_rect.x = x_pos+msk_x_offset;
				}
				x_vel = 0;
			}
		}
		x_pos += x_vel;
		// vertical collision
		bool grounded = false;
		for (SDL_Rect* s : collidables) {
			// check for collision on next frame
			if (Collision::placeMeeting(&msk_rect, s, 0, y_vel)) {
				int sign = 1;
				if (y_vel < 0) {sign = -1;}
				while (!Collision::placeMeeting(&msk_rect, s, 0, sign)) {
					y_pos += sign;
					msk_rect.y = y_pos+msk_y_offset;
				}
				y_vel = 0;
				// landed on tile
				if (sign == 1) {
					state = STATE_WALK;
					grounded = true;
				}
			}
			// supported on ground
			else if (y_vel == 0) {
				if (Collision::placeMeeting(&msk_rect, s, 0, 1)) {
					grounded = true;
				}
			}
		}
		if (!grounded) {state = STATE_JUMP;}
		y_pos += y_vel;

		// check for collectibles
		for (ObjectProp* p : e->p_list) {
			if (dynamic_cast<ObjectCandy*>(p) != nullptr) {
				if (Collision::collideRectangle(&msk_rect, p->get_msk())) {
					// update score etc.
					dynamic_cast<ObjectCandy*>(p)->collect(sector);
				}
			}
		}

		// check for fireball collisions - death
		for (ObjectEnemy* enemy : e->e_list) {
			if (dynamic_cast<ObjectFireball*>(enemy) != nullptr) {
				if (Collision::collideRectangle(&msk_rect, enemy->get_msk())) {
					dynamic_cast<ObjectFireball*>(enemy)->collect();
					dead = 1;
					state = STATE_DEATH;
				}
			}
		}
		
		// screen wrapping
		if (x_pos > GameEngine::SCREEN_WIDTH) {
			x_pos = -obj_rect.w;
		}
		if (x_pos < -obj_rect.w) {
			x_pos = GameEngine::SCREEN_WIDTH;
		}

		// fell out of world - death
		if (y_pos > GameEngine::SCREEN_HEIGHT+320) {
			dead = 1;
			state = STATE_DEATH;
		}

		// max out velocity
		if (x_vel > MAX_VEL) {x_vel = MAX_VEL;}
		else if (x_vel < -MAX_VEL) {x_vel = -MAX_VEL;}
	}

	obj_rect.x = x_pos;
	obj_rect.y = y_pos;
	msk_rect.x = x_pos+msk_x_offset;
	msk_rect.y = y_pos+msk_y_offset;
	// determines zone where next collectible appears
	sector = (msk_rect.x + 16 < (GameEngine::SCREEN_WIDTH * 0.5) ? 0 : 1);
	
	// detect new states and update sprites
	if (state != state_previous) {
		switch(state) {
			case STATE_IDLE:
				obj_sprite->spr_init("./assets/spr_pilgrim_idle.png", obj_renderer, 1, 0, 64, 64);
				break;
			case STATE_WALK:
				obj_sprite->spr_init("./assets/spr_pilgrim_walk.png", obj_renderer, 2, 175, 64, 64);
				break;
			case STATE_JUMP:
				obj_sprite->spr_init("./assets/spr_pilgrim_jump.png", obj_renderer, 1, 0, 64, 64);
				break;
			case STATE_DEATH:
				obj_sprite->spr_init("./assets/spr_pilgrim_death.png", obj_renderer, 1, 0, 64, 64);
				dead_off = y_pos-64;
				x_vel = 0;
				y_vel = 0;
				break;
			case STATE_REBIRTH:
				break;
		}
	}
	state_previous = state;
	x_prev = x_pos;
	y_prev = y_pos;
	}
}

void ObjectPlayer::obj_render() {
	SDL_SetRenderDrawColor(get_renderer(), 255, 0, 0, 1);
	if (dead != 2) {
		SDL_RenderCopyEx(get_renderer(), spr_texture, &spr_rect, &obj_rect, 0.0, NULL, spr_facing);
	}
}

SDL_Renderer* ObjectPlayer::get_renderer() {
	return obj_renderer;
}

Sprite* ObjectPlayer::get_sprite() {
	return obj_sprite;
}

double ObjectPlayer::get_x_vel() {return x_vel;}
double ObjectPlayer::get_y_vel() {return y_vel;}
void ObjectPlayer::set_x(int x) {x_pos = x;}
void ObjectPlayer::set_y(int y) {y_pos = y;}
int ObjectPlayer::get_x() {return x_pos;}
int ObjectPlayer::get_y() {return y_pos;}
void ObjectPlayer::set_x_vel(double xv) {x_vel = xv;} 
void ObjectPlayer::set_y_vel(double yv) {y_vel = yv;}
int ObjectPlayer::get_state() {return state;} 
void ObjectPlayer::set_state(int s) {state = s;}

void ObjectPlayer::reset() {
	x_pos = 128;
	y_pos = GameEngine::SCREEN_HEIGHT-192;
	x_vel = 0;
	y_vel = 0;
	state = STATE_IDLE;
	dead = 0;
}
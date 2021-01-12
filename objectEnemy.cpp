/*
  Daniel Ball
  objectEnemy.cpp
  CPSC 4160

  Witches and Fireballs
*/

#include "objectEnemy.h"
#include <SDL2/SDL_timer.h>
#include <math.h>

// instantiate base
ObjectEnemy::ObjectEnemy(SDL_Renderer* r, int x, int y, int w, int h) {
    obj_renderer = r;
    obj_sprite = new Sprite();
    spr_facing = SDL_FLIP_NONE;
    ObjectEnemy::obj_init(x, y, w, h);
}

ObjectEnemy::~ObjectEnemy() {}

void ObjectEnemy::obj_init(int x, int y, int w, int h) {
    obj_rect.x = x;
    obj_rect.y = y;
    obj_rect.w = w;
    obj_rect.h = h;

    x_pos = x;
    y_pos = y;
    x_prev = x_pos;
    y_prev = y_pos;
    x_vel = 0;
    y_vel = 0;
    state = 0;
    state_previous = 0;
}

void ObjectEnemy::obj_update(GameEngine* e) {
    spr_rect = obj_sprite->spr_update();
    spr_texture = obj_sprite->get_texture();
}

void ObjectEnemy::obj_render() {
    SDL_RenderCopyEx(get_renderer(), spr_texture, &spr_rect, &obj_rect, 0.0, NULL, spr_facing);
}

SDL_Renderer* ObjectEnemy::get_renderer() {
    return obj_renderer;
}
Sprite* ObjectEnemy::get_sprite() {
    return obj_sprite;
}
SDL_Rect* ObjectEnemy::get_msk() {
    return &msk_rect;
}

int ObjectEnemy::get_x(){return x_pos;}
int ObjectEnemy::get_y(){return y_pos;}
void ObjectEnemy::set_x(int x){x_pos = x;}
void ObjectEnemy::set_y(int y){y_pos = y;}
int ObjectEnemy::get_x_vel(){return x_vel;}
int ObjectEnemy::get_y_vel(){return y_vel;}
void ObjectEnemy::set_x_vel(int xv){x_vel = xv;}
void ObjectEnemy::set_y_vel(int yv){y_vel = yv;}
int ObjectEnemy::get_state(){return state;}
void ObjectEnemy::set_state(int s){state = s;}

/*********************
 * Witch
 *********************/
ObjectWitch::ObjectWitch(SDL_Renderer* r, int x, int y, int w, int h, int xoff, int yoff) : ObjectEnemy(r, x, y, w, h) {
    obj_sprite->spr_init("./assets/spr_witch.png", obj_renderer, 2, 300, 64, 80);
    msk_x_offset = xoff;
    msk_y_offset = yoff;
    msk_rect.x = x+msk_x_offset;
	msk_rect.y = y+msk_y_offset;
	msk_rect.w = 64;
	msk_rect.h = h-msk_y_offset-63;
    ObjectWitch::obj_init();
}

void ObjectWitch::obj_init() {
    base_y = y_pos;
    // offset for sinewave motion
    double yoff = (32*sin(x_pos*M_PI*0.01));
    y_pos = base_y + yoff;
    x_vel = base_vel = 2;
    state = STATE_GLIDE;
    state_previous = STATE_GLIDE;
    // timer for random fireball attacks
    timer = set_timer();
}

void ObjectWitch::obj_update(GameEngine* e) {
    ObjectEnemy::obj_update(e);

    // basic gliding movement
    if (state == STATE_GLIDE) {
        // offset for sinwave motion
        double yoff = (32*sin((x_pos*M_PI)/(100)));
        y_pos = base_y + yoff;

        // movement
        // right side of screen - reverse motion
        if (x_pos+obj_rect.w > (GameEngine::SCREEN_WIDTH-32) && x_vel > 0) {
            if (x_vel > 0.1) {
                x_vel = tween(x_vel, 0, 0.1);
            }
            else {
                x_vel = -base_vel;
                dir = -1;
            }
        }
        else 
        // left side of screen - reverse motion
        if (obj_rect.x < 32 && x_vel < 0) {
            if (x_vel < -0.1) {
                x_vel = tween(x_vel, 0, 0.1);
            }
            else {
                x_vel = base_vel;
                dir = 1;
            }
        }
        // time to deliver a fireball
        timer--;
        if (timer <= 0) {
            // only change on proper frame
            if (((int) (SDL_GetTicks() / 100) % 5) == 0) {
                state = STATE_CAST;
            }  
        }
    }
    else
    // casting a fireball
    if (state == STATE_CAST) {
        // animation ended, go back to moving
        if (((int) (SDL_GetTicks() / 100) % 5) >= 4) {
            state = STATE_GLIDE;
        }
        if (timer > 0) {
            // create fireball on proper frame
            if (((int) (SDL_GetTicks() / 100) % 5) >=3) {
                e->e_list.emplace_back(new ObjectFireball(get_renderer(), x_pos+12-16, y_pos+64-16));
                // target player position
                int target_x = e->get_player()->get_x()+64;
                int target_y = e->get_player()->get_y()+64;
                // occasionally throw to center
                if (rand()%10 < 2) {
                    target_x = GameEngine::SCREEN_WIDTH*0.5;
                    target_y = GameEngine::SCREEN_HEIGHT*0.5;
                }
                dynamic_cast<ObjectFireball*>(e->e_list.back())->obj_init(target_x, target_y);
                timer = 0;
             }
        }
    }
    else
    // play exiting animation
    if (state == STATE_EXIT) {
        y_pos = tween(y_pos, -320, 0.01);
        if ((-320 - y_pos) >= -160) {
            garbage = true;
            // engine can go to game over
            e->cleanUp();
        }
    }
    x_pos += x_vel;
    y_pos += y_vel;

    obj_rect.x = x_pos;
	obj_rect.y = y_pos;
    msk_rect.x = x_pos+msk_x_offset;
	msk_rect.y = y_pos+msk_y_offset;

    if (state != state_previous) {
		switch(state) {
            case STATE_IDLE:
                x_vel = 0;
                y_vel = 0;
                break;
            case STATE_GLIDE:
                base_vel = 2;
                x_vel = dir*base_vel;
                timer = set_timer();
                obj_sprite->spr_init("./assets/spr_witch.png", get_renderer(), 2, 300, 64, 80);
                break;
            case STATE_CAST:
                x_vel = 0;
                y_vel = 0;
                timer = 5;
                obj_sprite->spr_init("./assets/spr_witch_cast.png", get_renderer(), 5, 100, 64, 80);
                break;
            case STATE_EXIT:
                x_vel = 0;
                obj_sprite->spr_init("./assets/spr_witch.png", get_renderer(), 2, 300, 64, 80);
                break;
        }
    }

    state_previous = state;
    x_prev = x_pos;
    y_prev = y_pos;
}

void ObjectWitch::obj_render() {
    ObjectEnemy::obj_render();
    SDL_SetRenderDrawColor(get_renderer(), 255, 0, 0, 1);
}

/*********************
 * Fireball
 *********************/
ObjectFireball::ObjectFireball(SDL_Renderer* r, int x, int y) : ObjectEnemy(r, x, y, 32, 32) {
    obj_sprite->spr_init("./assets/spr_magic.png", obj_renderer, 2, 50, 16, 16);
    spr_rect = obj_sprite->spr_update();
    spr_texture = obj_sprite->get_texture();
    msk_x_offset = 6;
    msk_y_offset = 6;
    msk_rect.x = x+msk_x_offset;
	msk_rect.y = y+msk_y_offset;
	msk_rect.w = 20;
	msk_rect.h = 20;
}

ObjectFireball::~ObjectFireball() {}

// calculation motion for moving towards target
void ObjectFireball::obj_init(int tx, int ty) {
    int diff_x = abs(tx-(x_pos+16));
    int diff_y = abs(ty-(y_pos+16));
    double vec = sqrt((diff_x*diff_x + diff_y*diff_y));
    x_vel = (tx-(x_pos+16))/vec * base_vel;
    y_vel = (ty-(y_pos+16))/vec * base_vel;
}
void ObjectFireball::obj_update(GameEngine* e) {
    ObjectEnemy::obj_update(e);
    x_pos += x_vel;
    y_pos += y_vel;

    obj_rect.x = x_pos;
	obj_rect.y = y_pos;
    msk_rect.x = x_pos+msk_x_offset;
	msk_rect.y = y_pos+msk_y_offset;
    // cleanup if offscreen
    if (y_pos > GameEngine::SCREEN_HEIGHT) {
        garbage = true;
    }
}
void ObjectFireball::obj_render() {
    ObjectEnemy::obj_render();
}

void ObjectFireball::collect() {
    garbage = true;
}
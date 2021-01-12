/*
  Daniel Ball
  objectProp.cpp
  CPSC 4160

  Static Objects and Candy
*/

#include "objectProp.h"
#include <SDL2/SDL_timer.h>
#include <math.h>

ObjectProp::ObjectProp() {

}

ObjectProp::ObjectProp(SDL_Renderer* r, int x, int y, int w, int h) {
    obj_renderer = r;
    obj_sprite = new Sprite();
    spr_facing = SDL_FLIP_NONE;
    ObjectProp::obj_init(x, y, w, h);
}

ObjectProp::~ObjectProp() {}

void ObjectProp::obj_init(int x, int y, int w, int h) {
    obj_rect.x = x;
    obj_rect.y = y;
    obj_rect.w = w;
    obj_rect.h = h;

    x_pos = x;
    y_pos = y;
    x_vel = 0;
    y_vel = 0;
}

void ObjectProp::obj_update(GameEngine* e) {
    spr_rect = obj_sprite->spr_update();
    spr_texture = obj_sprite->get_texture();
}

void ObjectProp::obj_render() {
    SDL_RenderCopyEx(get_renderer(), spr_texture, &spr_rect, &obj_rect, 0.0, NULL, spr_facing);
}
SDL_Rect* ObjectProp::get_msk() {
    return &msk_rect;
}
SDL_Renderer* ObjectProp::get_renderer() {
    return obj_renderer;
}
Sprite* ObjectProp::get_sprite() {
    return obj_sprite;
}

void ObjectProp::cleanup() {
    garbage = true;
    obj_rect.x = 0;
    obj_rect.y = 0;
    msk_rect.x = 0;
    msk_rect.y = 0;
}

/*********************
 * Candy
 *********************/
ObjectCandy::ObjectCandy() : ObjectProp() {}
ObjectCandy::ObjectCandy(SDL_Renderer* r, int x, int y) : ObjectProp(r, x, y, 32, 32) {
    // actual candy sprite
    obj_sprite->spr_init("./assets/spr_candy.png", obj_renderer, 1, 0, 16, 16);
    ui_sprite = new Sprite();
    // score ui icon
    ui_sprite->spr_init("./assets/spr_candy_ui.png", obj_renderer, 1, 0, 16, 16);
    ui_texture = ui_sprite->get_texture();
    // score font sprite
    score_sprite->spr_init("./assets/spr_font.png", obj_renderer, 1, 0, 16, 160);
    score_texture = score_sprite->get_texture();
    msk_x_offset = 4;
    msk_y_offset = 2;
    msk_rect.x = obj_rect.x+msk_x_offset;
    msk_rect.y = obj_rect.y+msk_y_offset;
    msk_rect.w = 24;
    msk_rect.h = 30;
    ObjectCandy::obj_init();
}

ObjectCandy::~ObjectCandy() {
}

void ObjectCandy::obj_init() {
    base_y = y_pos;
    timer = 1;
    // left side of screen zone
    sector_1.x = 32;
    sector_1.y = 192;
    sector_1.w = 224;
    sector_1.h = 192;
    // right side of screen zone
    sector_2.x = 384;
    sector_2.y = 192;
    sector_2.w = 224;
    sector_2.h = 192;
    // ui icon
    ui_rect.x = 608;
    ui_rect.y = 448;
    ui_rect.w = 32;
    ui_rect.h = 32;
    // ui score
    score_rect.x = init_score_x;
    score_rect.y = 450;
    score_rect.w = 32;
    score_rect.h = 32;
    score_src_rect.x = 0;
    score_src_rect.y = 0;
    score_src_rect.w = 16;
    score_src_rect.h = 16;
}

void ObjectCandy::obj_update(GameEngine* e) {
    ObjectProp::obj_update(e);
    spr_rect = obj_sprite->spr_update();
	spr_texture = obj_sprite->get_texture();
    // bounce
    timer++;
    double yoff = (8*sin(timer*0.025));
    y_pos = base_y + yoff;
    obj_rect.x = x_pos;
	obj_rect.y = y_pos;
}

// render candy and score
void ObjectCandy::obj_render() {
    if (garbage == false) {
        ObjectProp::obj_render();
    }
    // score icon
    SDL_RenderCopyEx(get_renderer(), ui_texture, NULL, &ui_rect, 0.0, NULL, spr_facing);
    // draw the ones digit
    score_src_rect.x = off_ones * 16;
    SDL_RenderCopy(get_renderer(), score_texture, &score_src_rect, &score_rect);
    // draw the tens digit
    score_rect.x -= 16;
    score_src_rect.x = off_tens * 16;
    SDL_RenderCopy(get_renderer(), score_texture, &score_src_rect, &score_rect);
    score_rect.x = init_score_x;
}

// collision event
void ObjectCandy::collect(int sect) {
    int new_x;
    int new_y;
    // decide new spawn
    if (sect == 0) {
        new_x = sector_2.x+(rand() % sector_2.w);
        new_y = sector_2.y+(rand() % sector_2.h);
    }
    else {
        new_x = sector_1.x+(rand() % sector_1.w);
        new_y = sector_1.y+(rand() % sector_1.h);
    }
    x_pos = new_x;
    base_y = new_y;
    y_pos = base_y;
    obj_rect.x = x_pos;
    obj_rect.y = y_pos;
    msk_rect.x = obj_rect.x+msk_x_offset;
    msk_rect.y = obj_rect.y+msk_y_offset;
    // increase score, calculate digits
    count++;
    off_ones = (int)(count/pow(10,(/**/1-1))) % 10;
    off_tens = (int)(count/pow(10,(/**/2-1))) % 10;
}
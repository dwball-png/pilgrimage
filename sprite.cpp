/*
  Daniel Ball
  sprite.cpp
  CPSC 4160
*/

#include "sprite.h"
#include <SDL2/SDL_image.h>
#include <iostream>

Sprite::Sprite() {}

Sprite::~Sprite() {}

// initializes a new sprite from a provided image
void Sprite::spr_init(const char* path, SDL_Renderer* ren, int fc, int fd, int w, int h) {
	SDL_Surface* temp = IMG_Load(path);
	spr_texture = SDL_CreateTextureFromSurface(ren, temp);
	SDL_FreeSurface(temp);

	spr_rect.x = 0;
	spr_rect.y = 0;
	spr_rect.w = w;
	spr_rect.h = h;

	frame_count = fc;
	frame_duration = fd;
	previous_frame = 0;
}

// calculates the current frame and returns the updated sprite rectangle
SDL_Rect Sprite::spr_update() {
	if (frame_duration == 0) {
		return spr_rect;
	}
	int frame = (int) (SDL_GetTicks() / frame_duration) % frame_count;
	
	if (previous_frame != frame) {
		if (frame < 1) {
			spr_rect.x = 0;
		}
		else {
			spr_rect.x += spr_rect.w;
		}
	}
	previous_frame = frame;
	this_frame = frame;
	return spr_rect;
}

// returns the sprite's texture
SDL_Texture* Sprite::get_texture() {
	return spr_texture;
}

int Sprite::get_frame() {
	return this_frame;
}

int Sprite::get_frame_count() {
	return frame_count;
}
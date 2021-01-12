/*
  Daniel Ball
  gameBackground.cpp
  CPSC 4160
*/

class GameEngine;
#pragma once
#include "gameEngine.h"
#include "gameBackground.h"

GameBackground::GameBackground(char* path, SDL_Renderer* r, int srcx, int srcy, int srcw, int srch,
	int desx, int desy, int desw, int desh, int vx) {
	bck_renderer = r;
	SDL_Surface* temp = IMG_Load(path);
	spr_texture = SDL_CreateTextureFromSurface(bck_renderer, temp);
	SDL_FreeSurface(temp);
	
	bck_rect.x = desx;
	bck_rect.y = desy;
	bck_rect.w = desw;
	bck_rect.h = desh;

	spr_rect.x = srcx;
	spr_rect.y = srcy;
	spr_rect.w = srcw;
	spr_rect.h = srch;

	x_vel = vx;
}

GameBackground::~GameBackground() {}

void GameBackground::bck_render() {
	bck_rect.x += x_vel;
	if (bck_rect.x > GameEngine::SCREEN_WIDTH) {
		bck_rect.x = -GameEngine::SCREEN_WIDTH;
	}
	SDL_RenderCopy(bck_renderer, spr_texture, &spr_rect, &bck_rect);
}

void GameBackground::bck_quit() {}
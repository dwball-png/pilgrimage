/*
  Daniel Ball
  gameEngine.cpp
  CPSC 4160
*/

#include "gameEngine.h"

#include <iostream>

GameEngine::GameEngine() {}

GameEngine::~GameEngine() {}

// initializes the game engine
void GameEngine::init() {
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) { 
		std::cout << "Error initializing SDL: " << SDL_GetError() << std::endl; 
	}
	IMG_Init(IMG_INIT_PNG);
	TTF_Init();
	// create game window
	gameWindow = SDL_CreateWindow(
		"Pilgrimage",
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		SCREEN_WIDTH*1.0,
		SCREEN_HEIGHT*1.0,
		0
	);
	// create game renderer
	gameRenderer = SDL_CreateRenderer(gameWindow, -1, 0);
	// window icon
	SDL_Surface *ico_surface = IMG_Load("./assets/spr_icon.png");
	SDL_SetWindowIcon(gameWindow, ico_surface);
	SDL_FreeSurface(ico_surface);
	SDL_RenderSetScale(gameRenderer, 1.0, 1.0);
	// initialize input wrapper
	for (int i = 0; i < 300; i++) {
		keyboardInput[i] = 0;
	}
}

// create statics
void GameEngine::populate() {
	// create a player object
	ObjectPlayer* player = new ObjectPlayer(get_renderer(), 128, SCREEN_HEIGHT-188, 128, 128);
	player->get_sprite()->spr_init("./assets/spr_pilgrim_idle.png", player->get_renderer(), 1, 0, 64, 64);
	player->set_state(player->STATE_BEGIN);
	obj_list.push_back(player);

	// create backgrounds
	GameBackground* dark_background = new GameBackground("./assets/bck_dark.png", get_renderer(), 0, 0, 400, 240, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, 0);
	GameBackground* fog_background = new GameBackground("./assets/bck_fog.png", get_renderer(), 0, 0, 400, 240, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, 1);
	GameBackground* fog_background_wrap = new GameBackground("./assets/bck_fog.png", get_renderer(), 0, 0, 400, 240, -SCREEN_WIDTH, 0, SCREEN_WIDTH, SCREEN_HEIGHT, 1);
	GameBackground* tree_1 = new GameBackground("./assets/bck_ghost_trees.png", get_renderer(), 0, 0, 112, 224, 0, 0, 224, 448, 0);
	GameBackground* tree_2 = new GameBackground("./assets/bck_ghost_trees.png", get_renderer(), 112, 0, 128, 224, SCREEN_WIDTH-256, 0, 256, 448, 0);

	bck_list.push_back(dark_background);
	bck_list.push_back(fog_background);
	bck_list.push_back(fog_background_wrap);
	bck_list.push_back(tree_1);
	bck_list.push_back(tree_2);

	set_obj_player(player);
	GameLevel* l = new GameLevel();
	l->level_init(get_renderer());
	lvl = l;
}

// create entities
void GameEngine::entities() {
	// collectibles
	ObjectCandy* candy = new ObjectCandy(get_renderer(), 384+(rand() % 224), 192+(rand() %192));
	p_list.push_back(candy);
	// enemy
	ObjectWitch* witch = new ObjectWitch(get_renderer(), -128, 32, 128, 160, 32, 32);
	e_list.push_back(witch);
}

// handles quitting and keyboard input via input wrapper
void GameEngine::handle_events() {
	while (SDL_PollEvent(&gameInput) > 0) {
		switch (gameInput.type)
		{
			case SDL_QUIT:
				set_is_running(false);
			case SDL_KEYDOWN:
				switch (gameInput.key.keysym.sym) {
					case SDLK_RIGHT:
						keyboardInput[SDL_GetScancodeFromKey(SDLK_RIGHT)] = 1;
						break;
					case SDLK_LEFT:
						keyboardInput[SDL_GetScancodeFromKey(SDLK_LEFT)] = 1;
						break;
					case SDLK_x:
						keyboardInput[SDL_GetScancodeFromKey(SDLK_x)] = 1;
						break;
					case SDLK_r:
						if (keyboardInput[SDL_SCANCODE_R] == 0) {
							keyboardInput[SDL_GetScancodeFromKey(SDLK_r)] = 1;
						}
						break;
				}
				break;
			case SDL_KEYUP:
				switch (gameInput.key.keysym.sym) {
					case SDLK_RIGHT:
						keyboardInput[SDL_GetScancodeFromKey(SDLK_RIGHT)] = 0;
						break;
					case SDLK_LEFT:
						keyboardInput[SDL_GetScancodeFromKey(SDLK_LEFT)] = 0;
						break;
					case SDLK_x:
						keyboardInput[SDL_GetScancodeFromKey(SDLK_x)] = 0;
						break;
					case SDLK_r:
						keyboardInput[SDL_GetScancodeFromKey(SDLK_r)] = 0;
						break;
				}
				break;
			default:
				break;
		}
	}
}

// handles state changes and updates objects
void GameEngine::update_mechanics() {
	// if game has 'started'
	if (state == STATE_ACTIVE) {
		// calculate player object direction
		// 1 if right, -1 if left, 0 if both or neither
		int dir = keyboardInput[SDL_SCANCODE_RIGHT] - keyboardInput[SDL_SCANCODE_LEFT];
		if (obj_player->get_state() != obj_player->STATE_DEATH && obj_player->get_state() != obj_player->STATE_REBIRTH) {
			// player is grounded
			if (obj_player->get_state() != obj_player->STATE_JUMP)
			{
				// walking
				if (dir != 0) {
						obj_player->set_state(obj_player->STATE_WALK);
						obj_player->set_x_vel(obj_player->get_x_vel()+(1*dir));
				}
				// idle
				else {
					obj_player->set_state(obj_player->STATE_IDLE);
					if (obj_player->get_x_vel() > 0) {
						obj_player->set_x_vel(obj_player->get_x_vel()-0.5);
					}
					else if (obj_player->get_x_vel() < 0) {
						obj_player->set_x_vel(obj_player->get_x_vel()+0.5);
					}
				}
				// jumping
				if (keyboardInput[SDL_SCANCODE_X] == true) {
					obj_player->set_state(obj_player->STATE_JUMP);
					obj_player->set_y_vel(-obj_player->JUMP_VEL);
				}
			}
		}
	
		// restarting while playing, and for game over
		int res = keyboardInput[SDL_SCANCODE_R];
		if (res == 1) {
			keyboardInput[SDL_SCANCODE_R] = -1;
			clearEngine();
			populate();
			entities();
		}
	}
	else {
		// player has not begun game, wait for jump key pressed
		int x = keyboardInput[SDL_SCANCODE_X];
		if (x == true) {
			state = STATE_ACTIVE;
			entities();
			// remove title text
			t_list.clear();
		}
	}
	// update objects
	// player
	for (ObjectPlayer* a : obj_list) {
		a->obj_update(lvl, this);
	}
	// enemies
	for (int i = 0; i < e_list.size(); i++) {
		e_list[i]->obj_update(this);
		// witch exits stage if player dead
		if (dynamic_cast<ObjectWitch*>(e_list[i]) != nullptr) {
			if (obj_player->get_state() == obj_player->STATE_REBIRTH) {
				e_list[i]->set_state(ObjectWitch::STATE_EXIT);
			}
		}
		// cleanup for fireballs and witch
		if (e_list[i]->garbage == true) {
			delete e_list[i];
			e_list.erase(e_list.begin() + i);
		}
	}
	// collectibles
	for (int i = 0; i < p_list.size(); i++) {
		p_list[i]->obj_update(this);
		if (obj_player->get_state() == obj_player->STATE_REBIRTH) {
			p_list[i]->cleanup();
		}
	}
}

// clears screen, calls all render events, and presents
void GameEngine::render() {
	SDL_SetRenderDrawColor(gameRenderer, 64, 128, 64, 255);
	SDL_RenderClear(gameRenderer);
	for (GameBackground* b : bck_list) {
		b->bck_render();
	}
	lvl->level_render();
	for (int i = 0; i < e_list.size(); i++) {
		e_list[i]->obj_render();
	}
	for (ObjectProp* p : p_list) {
		p->obj_render();
	}
	for (ObjectPlayer* a : obj_list) {
		a->obj_render();
	}
	for (ObjectText* t : t_list) {
		t->obj_render();
	}
	SDL_RenderPresent(gameRenderer);
}

void GameEngine::quit() {
	SDL_DestroyRenderer(gameRenderer);
	SDL_DestroyWindow(gameWindow);
	IMG_Quit();
	SDL_Quit();
}

bool GameEngine::get_is_running() {
	return is_running;
}
void GameEngine::set_is_running(bool r) {
	is_running = r;
}
void GameEngine::set_obj_player(ObjectPlayer* g) {
	obj_player = g;
}

SDL_Window* GameEngine::get_window() {
	return gameWindow;
}
SDL_Renderer* GameEngine::get_renderer() {
	return gameRenderer;
}
SDL_Event GameEngine::get_input() {
	return gameInput;
}

ObjectPlayer* GameEngine::get_player() {
	return obj_player;
}

// clears update lists for restarting
void GameEngine::clearEngine() {
	for (ObjectPlayer* o : obj_list) {
		delete o;
	}
	obj_list.clear();
	for (ObjectEnemy* e : e_list) {
		delete e;
	}
	e_list.clear();
	for (GameBackground* b : bck_list) {
		delete b;
	}
	bck_list.clear();
	for (ObjectProp* p: p_list) {
		delete p;
	}
	p_list.clear();
	for (ObjectText* t : t_list) {
		delete t;
	}
	t_list.clear();
	delete lvl;
}

// game over screen
void GameEngine::cleanUp() {
	ObjectText* over = new ObjectText(get_renderer(), 320, 240, 24);
	ObjectText* restart = new ObjectText(get_renderer(), 320, 288, 12);
	over->obj_update("Game Over");
	restart->obj_update("-press R to restart-");
	t_list.push_back(over);
	t_list.push_back(restart);
}
/*
  Daniel Ball
  gameEngine.h
  CPSC 4160
*/

#pragma once
#include <SDL2/SDL.h> 
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_timer.h>
#include <vector>
#include "objectPlayer.h"
#include "objectEnemy.h"
#include "objectProp.h"
#include "gameBackground.h"
#include "objectText.h"
#include "gameLevel.h"

class GameEngine {
	public:
		GameEngine();
		~GameEngine();
		void init();
		void populate();
		void entities();
		void handle_events();
		void update_mechanics();
		void render();
		void quit();
		static const int SCREEN_WIDTH = 640;
		static const int SCREEN_HEIGHT = 480;
		bool get_is_running();
		void set_is_running(bool r);
		void set_obj_player(ObjectPlayer* g);

		std::vector<ObjectPlayer*> obj_list;
		std::vector<ObjectEnemy*> e_list;
		std::vector<GameBackground*> bck_list;
		std::vector<ObjectProp*> p_list;
		std::vector<ObjectText*> t_list;
		GameLevel* lvl;
		
		SDL_Window* get_window();
		SDL_Renderer* get_renderer();
		SDL_Event get_input();

		static const int STATE_BEGIN = 0;
		static const int STATE_ACTIVE = 1;

		ObjectPlayer* get_player();
		void cleanUp();
	private:
		bool is_running = true;
		SDL_Window* gameWindow;
		SDL_Renderer* gameRenderer;
		SDL_Event gameInput;
		short keyboardInput[300];
		ObjectPlayer* obj_player;
		unsigned int last = 0, curr;
		void clearEngine();
		int state = 0;
};

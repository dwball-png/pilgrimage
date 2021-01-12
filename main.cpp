/*
  Daniel Ball
  main.cpp
  CPSC 4160
*/

#include "gameEngine.cpp"
#include "objectPlayer.cpp"
#include "sprite.cpp"
#include "gameBackground.cpp"

#include "gameLevel.cpp"
#include "objectEnemy.cpp"
#include "objectProp.cpp"
#include "objectText.cpp"
#include "time.h"

int main(int argc, char* args[]) 
{ 
	// seed randomization
	srand(time(NULL));
	GameEngine* engine = new GameEngine();
	engine->init();
	const int FPS = 60;
	const int FRAME_DURATION = 1000 / FPS;
	int time_start;
	int time_duration;
	
	// create statics
	engine->populate();

	// create start text
	ObjectText* pilgrim = new ObjectText(engine->get_renderer(), 320, 240, 24);
	ObjectText* start = new ObjectText(engine->get_renderer(), 320, 288, 12);
	pilgrim->obj_update("Pilgrimage");
	start->obj_update("-press X-");
	engine->t_list.push_back(pilgrim);
	engine->t_list.push_back(start);

	// game loop
	while (engine->get_is_running()) {
		time_start = SDL_GetTicks();
		engine->handle_events();
		engine->update_mechanics();
		engine->render();
		time_duration = SDL_GetTicks() - time_start;
		if (time_duration < FRAME_DURATION) {
			SDL_Delay(FRAME_DURATION - time_duration);
		}
	}
	engine->quit();
	return 0; 
}
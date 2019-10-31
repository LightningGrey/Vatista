#include "Log.h"
#include "Game.h"

int main() {
	
	Vatista::Log::Init();
	VATISTA_LOG_INFO("Vatista.dll running.");
	
	Vatista::Game* game = new Vatista::Game();
	game->run();
	delete game;

	Vatista::Log::Uninitialize();
	return 0;
}
/******* Vatista *******/

/*
Build 1.0
Date: November 28th, 2019
Authors: DHC Studios
*/

#include "Utilities/Log.h"
#include "Game/Game.h"

int main() {

	//init and game creation
	Vatista::Log::Init();
	VATISTA_LOG_INFO("Vatista.dll running.");
	
	Vatista::Game* game = new Vatista::Game();
	game->run();
	delete game;

	Vatista::Log::Uninitialize();
	return 0;
}
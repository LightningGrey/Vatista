/******* Vatista *******/

/*
Build 1.0
Date: November 28th, 2019
Authors: DHC Studios
*/

#include "Utilities/Log.h"
#include "Game/sceneLayers/Game.h"
#include "Game/sceneLayers/TitleLayer.h"
#include "Utilities/EnumToGameState.h"
#include "Game/sceneLayers/MenuLayer.h"

int main() {

	//init and game creation
	Vatista::Log::Init();
	VATISTA_LOG_INFO("Vatista.dll running.");
	
	
	Vatista::EnumToGameState::Gamestate currentscene = Vatista::EnumToGameState::Gamestate::mainMenu;
	Vatista::EnumToGameState SetGameState(Vatista::EnumToGameState::Gamestate Gamestate_);

	Vatista::TitleLayer* titleScreenScene = new Vatista::TitleLayer();
	
	Vatista::MenuLayer* stage1Scene = new Vatista::MenuLayer();

	switch (currentscene)
	{
	case Vatista::EnumToGameState::Gamestate::mainMenu:
		
		titleScreenScene->run();
		delete titleScreenScene;
		break;

	case Vatista::EnumToGameState::Gamestate::stage1:
	
		stage1Scene->run();
		delete stage1Scene;

		break;
	case Vatista::EnumToGameState::Gamestate::stage2:
		// ...
		break;
	}



	
	//Vatista::TitleLayer* title = new Vatista::TitleLayer();
	//title->run();
	//delete title;

	Vatista::Log::Uninitialize();
	return 0;
}
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
#include "Graphics/Window.h"



int main() {

	//init and game creation
	Vatista::Log::Init();
	VATISTA_LOG_INFO("Vatista.dll running.");
	
	
	Vatista::EnumToGameState::Gamestate currentscene = Vatista::EnumToGameState::Gamestate::mainMenu;
	Vatista::EnumToGameState SetGameState(Vatista::EnumToGameState::Gamestate Gamestate_);

	Vatista::TitleLayer* titleScreenScene = new Vatista::TitleLayer();
	
	Vatista::MenuLayer* menuScene = new Vatista::MenuLayer();

	Vatista::Game* stage1Scene = new Vatista::Game();

	//Vatista::Game2* stage2Scene = new Vatista::Gane2();

	//Vatista::Window* gameWindow1;

	//int buttonSelected = 0;

	//if (glfwGetKey(gameWindow1->getWindow(), GLFW_KEY_W) == GLFW_PRESS) {
	//	buttonSelected++;
	//}


	switch (currentscene)
	{
	case Vatista::EnumToGameState::Gamestate::mainMenu:
		
		titleScreenScene->run();
		delete titleScreenScene;
		break;

	case Vatista::EnumToGameState::Gamestate::selectionMenu:

		menuScene->run();
		delete menuScene;

		break;

	case Vatista::EnumToGameState::Gamestate::stage1:
	
		stage1Scene->run();
		delete menuScene;

		break;
	case Vatista::EnumToGameState::Gamestate::stage2:
		
	/*	stage2Scene->run();
		delete stage2Scene;*/
		break;
	}

	
	//Vatista::TitleLayer* title = new Vatista::TitleLayer();
	//title->run();
	//delete title;

	Vatista::Log::Uninitialize();
	return 0;
}
#pragma once
#include <algorithm>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <vector>
#include <cctype>

namespace Vatista {

	class EnumToGameState {
	public:
		enum Gamestate : char
		{
			mainMenu = 0, stage1 = 1, stage2 = 2
		};

	private:
		Gamestate stagelevel = mainMenu;
	public:
		void SetGameState(Gamestate Gamestate_){
			stagelevel = Gamestate_;
		}

	};

}
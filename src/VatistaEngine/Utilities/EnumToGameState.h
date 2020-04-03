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

		
		enum Gamestate
		{
			mainMenu = 0, selectionMenu = 1, stage1 = 2, stage2 = 3
		};


	private:
		Gamestate stagelevel;
	public:
		void SetGameState(Gamestate Gamestate_){
			stagelevel = Gamestate_;
		}

	};

}
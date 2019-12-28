#pragma once

/** Gathers Under Night **/
//player class
//attach to each port
//bind character to player
//track win counter

#include "Character.h"
#include "Utils.h"

namespace Vatista {
	class Player {
	public:
		SharedPtr(Player);
		Player();
		~Player();

		//round wins per player
		void incrementRoundWins() { roundCount++; };
		int getRoundWins() { return roundCount; };

		//match wins per player
		void incrementMatchWins() { winCount++; };
		int getMatchWins() { return winCount; };

		void bindCharacter(Vatista::Character::Sptr character);
		Vatista::Character::Sptr getCharacter();

		//win condition check
		//if win, reset rounds for each player to 0 and increment winner's match win count
		void winCon();
	private:
		Vatista::Character::Sptr currentChar;
		int roundCount;
		int winCount;
	};
}
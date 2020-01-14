#include "Player.h"


void Vatista::Player::bindCharacter(Vatista::Character::Sptr character)
{
	currentChar = character;
}

Vatista::Character::Sptr Vatista::Player::getCharacter()
{
	return currentChar;
}

void Vatista::Player::winCon()
{
	if (roundCount == 2) {
		incrementMatchWins();
	}
}

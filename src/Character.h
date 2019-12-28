#pragma once

//character class
//backbone of every character

#include <vector>
#include "GameObject.h"
#include "Utils.h"

//possible states of each character 
enum class State {
	IDLE,
	WALKING,
	DASHING,
	RUNNING,
	BLOCKING,
	ATTACKING,
	HITSTUN,
	DEATH
};

namespace Vatista {
	class Character : public GameObject {
	public:
		SharedPtr(Character);
		Character() {};
		~Character() {};

		virtual void movement(int keyPress) = 0;
		virtual void attack() = 0;
		virtual void collision() = 0;

	protected:
		//animation data
		std::vector<Vatista::Mesh::Sptr> modelClips;

		//global chararcter variables
		float moveSpeed;
		
		//health data
		//Vatista::Health::Sptr health;

		//attack data
		//Vatista::Attack::Sptr lightAttack;
		//Vatista::Attack::Sptr heavyAttack;

		//hitbox data
		//Vatista::Hitbox::Sptr hitbox;

		//associated theme
		//std::string themeName;

		//associated stage
		//std::string stageName;

		//morphing variables
		float dt = 0.0167f;
		float morph = 0.0f;
		float currentTime = 0.0f;
		float endTime = 10.0f;

		//current state
		State state = State::IDLE;
	};
}
#pragma once

#include "Mesh.h"

namespace Vatista {
	class Player {
	public:
		Player();
		~Player();
	private:
		glm::vec3 position;

		//idle, walking, jumping, blocking, hitstun
		bool state;
	};
}
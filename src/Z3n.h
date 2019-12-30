#pragma once
#include "Character.h"
#include "Controls.h"

namespace Vatista {
	class Z3n : public Character {
	public:
		SharedPtr(Z3n);
		Z3n();
		~Z3n();

		void movement(int keyPress, Controls button);
		void attack(int keyPress, Controls button);
		void collision();
		void update(float dt, Controls key);
	};
}
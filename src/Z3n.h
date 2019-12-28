#pragma once
#include "Character.h"

namespace Vatista {
	class Z3n : public Character {
	public:
		SharedPtr(Z3n);
		Z3n();
		~Z3n();

		void movement(int keyPress);
		void attack(int keyPress);
		void collision();
	};
}
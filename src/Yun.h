#pragma once
#include "Player.h"

namespace Vatista {
	class Yun : public Player {
	public:
		Yun();
		~Yun();
	private:
		float moveSpeed;
	};
}
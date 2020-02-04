#pragma once
#include "GameObject.h"
#include <GLFW/glfw3.h> 
#include "Utils.h"
#include <iostream> 

namespace Vatista{
	class Stage : public GameObject {
	public:
		SharedPtr(Stage);
		Stage();
		std::vector<GameObject::Sptr> Props;
	};
}
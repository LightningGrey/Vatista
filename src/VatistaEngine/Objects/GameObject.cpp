#include "GameObject.h"
#include <GLM\include\GLM\gtc\matrix_transform.hpp>

namespace Vatista {
	bool GameObject::collisionCheck(glm::vec3 x, glm::vec2 collider1, glm::vec3 y, glm::vec2 collider2)
	{
		//std::cout << "           obj1.z = " << obj1.z << std::endl;
		//std::cout << "                         obj2.z = " << obj2.z << std::endl;
		if (x.x < y.x + collider2.x && x.x + collider1.x > y.x&& x.y < y.y + collider2.y && x.y + collider1.y > y.y) {
			return true;
		}
		else
			return false;

	}
}
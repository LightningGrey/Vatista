#pragma once
#include "GLM/glm.hpp"
#include "Mesh.h"
#include "Objects/Stationary/StationaryObj.h"

namespace Vatista {
	class Light : public StationaryObj {
	public:
		Light();
		~Light();

		void setColour(glm::vec3 col) { colour = col; };
		glm::vec3 getColour() { return colour; };

		void setAttenuation(float atten) { attenuation = atten; };
		float getAttenuation() { return attenuation };

	private:
		glm::vec3 colour;
		float attenuation;
	};
}
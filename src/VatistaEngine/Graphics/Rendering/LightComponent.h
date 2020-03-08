#pragma once
#include "GLM/glm.hpp"
#include "Mesh.h"
#include "Objects/Stationary/StationaryObj.h"
#include "Graphics/Window.h"

namespace Vatista {
	class Light : public StationaryObj {
	public:
		SharedPtr(Light);
		Light();
		~Light();

		void setColour(glm::vec3 col) { colour = col; };
		glm::vec3 getColour() { return colour; };

		void setAttenuation(float atten) { attenuation = atten; };
		float getAttenuation() { return attenuation; };

		void setControl(bool ctrl) { control = ctrl; };
		float getControl() { return control; };

		void transform();
		void controller(Window* window);

	private:
		glm::vec3 colour;
		float attenuation;

		bool control = false;

		float total = 0.0f;
		float inc = 0.01f;
	};
}
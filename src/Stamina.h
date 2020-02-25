#pragma once
#include "GameObject.h"
#include "Mesh.h"
#include "Material.h"
#include "Camera.h"
#include "Utils.h"

namespace Vatista {
	class Stamina : public GameObject {
	public:
		SharedPtr(Stamina);

		Stamina();
		~Stamina();

		void Draw(const Vatista::Camera::Sptr& camera);

		//change later
		float getStamina() { return stamina; }
		void setStamina(float stam) { stamina = stam; }

	private:
		//change later
		float stamina = 0.0f;
	};
}
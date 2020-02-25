#pragma once
#include "GameObject.h"
#include "Mesh.h"
#include "Material.h"
#include "Camera.h"
#include "Utils.h"

namespace Vatista {
	class StationaryObj : public GameObject {
	public:
		SharedPtr(StationaryObj);

		StationaryObj();
		~StationaryObj();

		void Draw(const Vatista::Camera::Sptr& camera);
	};
}
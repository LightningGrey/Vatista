#pragma once
#include "Material.h"
#include "Mesh.h"
struct MeshRenderer {
	Vatista::Material::Sptr Material;
	Vatista::Mesh_sptr Mesh;
};
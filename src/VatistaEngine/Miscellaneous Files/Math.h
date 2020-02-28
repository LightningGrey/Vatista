#pragma once
#include <vector>
#include "GLM/glm.hpp"
#include "Graphics/Object Graphics/Mesh.h"

namespace Vatista {
	struct Math {
		static void computeTangents(std::vector<Vertex>& verts, std::vector<uint32_t>& indices,
			std::vector<glm::vec3>& tangents, std::vector<glm::vec3>& bitangents) {

			for (int i = 0; i < verts.size(); i + 3) {
				//face vertices
				glm::vec3& v0 = verts[indices[i]].Position;
				glm::vec3& v1 = verts[indices[i + 1]].Position;
				glm::vec3& v2 = verts[indices[i + 2]].Position;
				//face UVs
				glm::vec2& uv0 = verts[indices[i]].UV;
				glm::vec2& uv1 = verts[indices[i + 1]].UV;
				glm::vec2& uv2 = verts[indices[i + 2]].UV;

				//edges + UV delta
				glm::vec3 edge1 = v1 - v0;
				glm::vec3 edge2 = v2 - v1;
				glm::vec2 deltaUV1 = uv1 - uv0;
				glm::vec2 deltaUV2 = uv2 - uv0;

				float r = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV1.y * deltaUV2.x);
				glm::vec3 tangent = (edge1 * deltaUV2.y - edge2 * deltaUV1.y) * r;
				glm::vec3 bitangent = (edge2 * deltaUV1.x - edge1 * deltaUV2.x) * r;

				tangents.push_back(tangent);
				bitangents.push_back(bitangent);
			}

		};

		static void computeTangents(std::vector<MorphVertex>& verts, std::vector<uint32_t>& indices,
			std::vector<glm::vec3>& tangents, std::vector<glm::vec3>& bitangents) {

			for (int i = 0; i < verts.size(); i + 3) {
				//face vertices
				glm::vec3& v0 = verts[indices[i]].Position;
				glm::vec3& v1 = verts[indices[i + 1]].Position;
				glm::vec3& v2 = verts[indices[i + 2]].Position;
				//face UVs
				glm::vec2& uv0 = verts[indices[i]].UV;
				glm::vec2& uv1 = verts[indices[i + 1]].UV;
				glm::vec2& uv2 = verts[indices[i + 2]].UV;

				//edges + UV delta
				glm::vec3 edge1 = v1 - v0;
				glm::vec3 edge2 = v2 - v1;
				glm::vec2 deltaUV1 = uv1 - uv0;
				glm::vec2 deltaUV2 = uv2 - uv0;

				float r = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV1.y * deltaUV2.x);
				glm::vec3 tangent = (edge1 * deltaUV2.y - edge2 * deltaUV1.y) * r;
				glm::vec3 bitangent = (edge2 * deltaUV1.x - edge1 * deltaUV2.x) * r;

				tangents.push_back(tangent);
				bitangents.push_back(bitangent);
			}

		};
	};
}
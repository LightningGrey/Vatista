#include "Material.h"

namespace Vatista {
	void Material::Apply() {
		for (auto& kvp : myMat4s)
			myShader->SetUniform(kvp.first.c_str(), kvp.second);
		for (auto& kvp : myVec4s)
			myShader->SetUniform(kvp.first.c_str(), kvp.second);
		for (auto& kvp : myVec3s)
			myShader->SetUniform(kvp.first.c_str(), kvp.second);
		for (auto& kvp : myFloats)
			myShader->SetUniform(kvp.first.c_str(), kvp.second);

		int slot = 0;   
		for (auto& kvp : myTextures) { 
			kvp.second->bind(slot);    
			myShader->SetUniform(kvp.first.c_str(), slot);    
			slot++; 
		}
	}
}
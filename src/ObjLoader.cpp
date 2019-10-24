//#include <stdio.h>
#include <string>
#include <cstring>
#include <iostream>
#include <fstream>

#include "ObjLoader.h"
#include "Log.h"

namespace Vatista {
	bool ObjLoader::load(std::string filename, std::vector<glm::vec3>& objVertices,
		std::vector<glm::vec2>& objUVs, std::vector<glm::vec3>& objNormals,
		std::vector<uint32_t>& objIndices)
	{
		std::cout << "Object loading..." << std::endl;

		//temp vectors for values
		std::vector<unsigned int> vertexIndices, uvIndices, normalIndices;
		std::vector<glm::vec3> tempVertices;
		std::vector<glm::vec2> tempUVs;
		std::vector<glm::vec3> tempNormals;

		//file load
		std::ifstream file;
		file.open(filename, std::ios::binary);

		// error on file load
		if (!file) {
			VATISTA_LOG_ERROR("No file");
			throw new std::runtime_error("File open failed");
			return false;
		}

		//read line, take vertex/uv/normal/index data
		std::string readingLine;
		while (std::getline(file, readingLine)) {
			if (strstr(readingLine.c_str(), "#"))
				continue;
			else if (strstr(readingLine.c_str(), "vt ")) {
				glm::vec2 uv;
				sscanf_s(readingLine.c_str(), "vt %f %f\n", &uv.x, &uv.y);
				tempUVs.push_back(uv);
			}
			else if (strstr(readingLine.c_str(), "vn ")) {
				glm::vec3 normal;
				sscanf_s(readingLine.c_str(), "vn %f %f %f\n", &normal.x, &normal.y, &normal.z);
				tempNormals.push_back(normal);
			}
			else if (strstr(readingLine.c_str(), "v ")) {
				glm::vec3 vertex;
				sscanf_s(readingLine.c_str(), "v %f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
				tempVertices.push_back(vertex);
			}
			else if (strstr(readingLine.c_str(), "f ")) {
				std::string vertex1, vertex2, vertex3;
				unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
				sscanf_s(readingLine.c_str(), "f %d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0],
					&uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1],
					&normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2]);

				vertexIndices.push_back(vertexIndex[0] - 1);
				vertexIndices.push_back(vertexIndex[1] - 1);
				vertexIndices.push_back(vertexIndex[2] - 1);
				uvIndices.push_back(uvIndex[0] - 1);
				uvIndices.push_back(uvIndex[1] - 1);
				uvIndices.push_back(uvIndex[2] - 1);
				normalIndices.push_back(normalIndex[0] - 1);
				normalIndices.push_back(normalIndex[1] - 1);
				normalIndices.push_back(normalIndex[2] - 1);
			}
		}

		//moves values into obj variables
		for (unsigned int i = 0; i < tempVertices.size(); i++) {
			objVertices.push_back(tempVertices[i]);
		}
		for (unsigned int i = 0; i < tempUVs.size(); i++) {
			objUVs.push_back(tempUVs[i]);
		}
		for (unsigned int i = 0; i < tempNormals.size(); i++) {
			objNormals.push_back(tempNormals[i]);
		}

		for (unsigned int i = 0; i < vertexIndices.size(); i++) {
			objIndices.push_back(vertexIndices[i]);
		}

		file.close();
		return true;
	}
}
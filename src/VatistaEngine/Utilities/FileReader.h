#pragma once
#include <fstream>
#include <vector>
#include <string>
#include "Log.h"
#include "Graphics/Rendering/Mesh.h"

namespace Vatista {
	struct FileReader {
		//read lines of text file
		static bool readLines(std::string filename, std::vector<std::string>& dataList) {
		
			std::fstream loadingFile;
			std::string line;

			loadingFile.open(filename, std::ios::in | std::ios::binary);
			if (!loadingFile) {
				VATISTA_LOG_ERROR("No file");
				throw new std::runtime_error("File open failed");
				return false;
			}

			while (std::getline(loadingFile, line)) {
				if (!line.empty() && line[line.size() - 1] == '\r')
					line.pop_back();
				dataList.push_back(line); 
			}
			loadingFile.close();
			return true;
		}

		//reads vsf and converts to mesh
		static bool vsfRead(std::string filename, Vatista::Mesh::Sptr &mesh) {

			std::fstream dataFile;

			dataFile.open("./res/Objects/" + filename, std::ios::in | std::ios::binary);
			if (!dataFile) {
				VATISTA_LOG_ERROR("No file");
				throw new std::runtime_error("File open failed");
				return false;
			}

			int animBuffer; //check for static obj vs moving 
			int vertTotalBuffer; //check for verts total 
			int indiceTotalBuffer; //check for indices total 
			LoadMorphVertex* morphBuffer;
			uint32_t* indicesBuffer;

			std::cout << "reading now" << std::endl;

			dataFile.read((char*)&animBuffer, sizeof(int));
			dataFile.read((char*)&vertTotalBuffer, sizeof(int));
			dataFile.read((char*)&indiceTotalBuffer, sizeof(int));

			morphBuffer = new LoadMorphVertex[vertTotalBuffer];
			indicesBuffer = new uint32_t[indiceTotalBuffer];

			//access using pointer (*(value+n)) 
			dataFile.read((char*)morphBuffer, sizeof(LoadMorphVertex) * vertTotalBuffer);
			dataFile.read((char*)indicesBuffer, sizeof(uint32_t) * indiceTotalBuffer);

			dataFile.close();

			//vertex data
			std::vector<uint32_t> indices;
			std::vector<Vertex> vertData;
			std::vector<MorphVertex> morphVertData;

			////tangents
			//std::vector<glm::vec3> tangents;
			//std::vector<glm::vec3> bitangents;
			//
			////Math::computeTangents(morphVertData, indices, tangents, bitangents);
			for (int j = 0; j < vertTotalBuffer; j++) {
				MorphVertex morph = { Vertex{(*(morphBuffer + j)).Position,
				(*(morphBuffer + j)).UV, (*(morphBuffer + j)).Normal}, (*(morphBuffer + j)).PositionS,
					(*(morphBuffer + j)).NormalS };
				morphVertData.push_back(morph);
			}


			for (int k = 0; k < indiceTotalBuffer; k++) {
				indices.push_back(*(indicesBuffer + k));
			}

			mesh = std::make_shared<Mesh>(indices, indices.size(),
				morphVertData, morphVertData.size());

		}

		//reads vsf and converts to mesh
		static bool vsfRead2(std::string filename, Vatista::Mesh::Sptr& mesh) {

			std::fstream dataFile;

			dataFile.open("./res/Objects/" + filename, std::ios::in | std::ios::binary);
			if (!dataFile) {
				VATISTA_LOG_ERROR("No file");
				throw new std::runtime_error("File open failed");
				return false;
			}

			int animBuffer; //check for static obj vs moving 
			int vertTotalBuffer; //check for verts total 
			int indiceTotalBuffer; //check for indices total 
			LoadMorphVertex2* morphBuffer;
			uint32_t* indicesBuffer;

			std::cout << "reading now" << std::endl;

			dataFile.read((char*)&animBuffer, sizeof(int));
			dataFile.read((char*)&vertTotalBuffer, sizeof(int));
			dataFile.read((char*)&indiceTotalBuffer, sizeof(int));

			morphBuffer = new LoadMorphVertex2[vertTotalBuffer];
			indicesBuffer = new uint32_t[indiceTotalBuffer];

			//access using pointer (*(value+n)) 
			dataFile.read((char*)morphBuffer, sizeof(LoadMorphVertex2) * vertTotalBuffer);
			dataFile.read((char*)indicesBuffer, sizeof(uint32_t) * indiceTotalBuffer);

			dataFile.close();

			//vertex data
			std::vector<uint32_t> indices;
			std::vector<Vertex> vertData;
			std::vector<MorphVertex2> morphVertData;

			////tangents
			//std::vector<glm::vec3> tangents;
			//std::vector<glm::vec3> bitangents;
			//
			////Math::computeTangents(morphVertData, indices, tangents, bitangents);
			for (int j = 0; j < vertTotalBuffer; j++) {
				MorphVertex2 morph = {Vertex{(*(morphBuffer + j)).Position,
				(*(morphBuffer + j)).UV, (*(morphBuffer + j)).Normal}, (*(morphBuffer + j)).PositionS,
					(*(morphBuffer + j)).NormalS, (*(morphBuffer + j)).Tangent, (*(morphBuffer + j)).TangentS };
				morphVertData.push_back(morph);
			}


			for (int k = 0; k < indiceTotalBuffer; k++) {
				indices.push_back(*(indicesBuffer + k));
			}

			mesh = std::make_shared<Mesh>(indices, indices.size(),
				morphVertData, morphVertData.size());

		}
	};
}

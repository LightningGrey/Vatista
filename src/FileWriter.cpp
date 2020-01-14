#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "GLM/include/GLM/glm.hpp"
#include "ObjLoader.h"

//main 
int main()
{
	//variables
	Vatista::ObjLoader loader;
	std::vector<uint32_t> indices;
	std::vector<Vertex> vertData;
	std::vector<uint32_t> indices2;
	std::vector<Vertex> vertData2;
	std::vector<MorphVertex> morphVertData;
	std::vector<MorphVertex2> morphData2; //to pass into file
	
	std::string file1;
	std::string file2;
	std::string finalFile; //final file name, based on first file
	bool loadCheck = false;
	bool loadCheck2 = false;

	//file to write to
	std::fstream wfile;


	//value to determine if object moves
	//0 for static object, 1 for moving object
	int animCheck = 1;
	//index total
	int indiceTotal;


	//input for files
	std::cout << "------------------Sidereus Fragmentum------------------" << std::endl;
	std::cout << "Enter the name of the file/beginning file of animation: " << std::endl;
	std::cin >> file1;
	std::cout << "If model is animated, enter name of file:" << std::endl;
	std::cout << "Otherwise, enter \"N/A\"" << std::endl;
	std::cin >> file2;
	finalFile = file1.substr(0, file1.length()-3) + "vsf";

	//loader
	loadCheck = loader.load(file1, indices, vertData);
	if (file2.compare("N/A") == 0) {
		animCheck = 0;
	}
	else {
		loadCheck2 = loader.load(file2, indices2, vertData2);
	}
	
	//if loads failed, close
	if (loadCheck2 != true){
		if (loadCheck != true)
			return 1;
	}

	if (animCheck == true) {
		for (int i = 0; i < vertData2.size(); i++) {
			morphVertData.push_back(MorphVertex((vertData2[i]),
				vertData[i].Position, vertData[i].Normal));
		}

		for (int i = 0; i < morphVertData.size(); i++) {
			morphData2.push_back(MorphVertex2{ morphVertData[i].Position,
				morphVertData[i].UV, morphVertData[i].Normal,
				morphVertData[i].PositionS, morphVertData[i].NormalS });
		}
	}

	wfile.open(finalFile, std::ios::out |
		std::ios::binary | std::ios::trunc);

	if (!wfile) {
		std::cout << "Cannot open file!" << std::endl;
		return 1;
	}

	std::cout << "writing now" << std::endl;
	
	//write data
	wfile.write((char*)&animCheck, sizeof(size_t));
	wfile.write((char*)&indiceTotal, sizeof(size_t));

	//check is true
	if (animCheck == true) {
		for (int i = 0; i < morphData2.size(); i++) {
			wfile.write((char*)&morphData2[i], sizeof(MorphVertex2));
		}
	}
	else {
		for (int i = 0; i < vertData.size(); i++) {
			wfile.write((char*)&vertData[i], sizeof(Vertex));
		}
	}
	
	for (int i = 0; i < indices2.size(); i++) {
		wfile.write((char*)&indices[i], sizeof(uint32_t));
	}
	
	wfile.seekg(0, std::ios::end);
	int length = wfile.tellg();
	
	wfile.close();

	std::cout << "File writing successful. Output can be found as " + file1 +
		".txt" << std::endl;
	std::cout << "Shutting down Sidereus Fragmentum..." 
		<< std::endl;

	std::cin.get();
	return 0;
}
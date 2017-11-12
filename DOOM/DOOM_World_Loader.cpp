
#include "DOOM_World_Loader.hpp"

#include "../OBJ_Loader.hpp"

#include <iostream>

DOOM_WorldLoader::DOOM_WorldLoader(){
	
}

DOOM_World* DOOM_WorldLoader::loadLevel(std::string fileName){
	std::ifstream file;
	file.open(fileName);
	if(!file.is_open() || !file.good()){
		std::cerr << "Error while loading level " << fileName << std::endl;
		return NULL;
	}

	Texture* activeTexture = nullptr;
	DOOM_World* world = new DOOM_World();

	std::string line;
	while(std::getline(file, line)){
		if(!line.substr(0,1).compare("#"))
			continue;
		std::vector<std::string> data = OBJ_Loader::split(line, ' ');
		if(!line.substr(0,6).compare("level ")){

		}else if(!line.substr(0,8).compare("texture ")){
		  if(data[1] == "add"){
				//Declaration of a texture to load
				std::vector<std::string> association = OBJ_Loader::split(data[2], '=');
				
				Texture* texture = new Texture(association[1]);
				textures.insert(std::pair<std::string, Texture*>(association[0], texture));
			}else{
				std::vector<std::string> association = OBJ_Loader::split(data[1], '=');
				if(association[0] == "active"){
					//Active Texture
					activeTexture = textures[association[1]];
					world->setActiveTexture(activeTexture);
				}
			}
		}else if(!line.substr(0,5).compare("wall ")){
			std::vector<Vec3> vertices(2);
			Vec3 normal;
			float height = 0;
			int size;
			for(int i = 1; i < data.size(); ++i){
				std::vector<std::string> entry = OBJ_Loader::split(data[i], ',');
				if(entry.size() == 2){
					//2D position
					size = 2;
					if(i == 3){
						normal = Vec3(std::stof(entry[0]), 0, std::stof(entry[1]));
					}else{
						vertices[i-1] = Vec3(std::stof(entry[0]), height, std::stof(entry[1]));
					}
				}else if(entry.size() == 3){
					//3D position
					size = 3;
					if(i == 3){
						normal = Vec3(std::stof(entry[0]), std::stof(entry[1]), std::stof(entry[2]));
					}else{
						vertices[i-1] = Vec3(std::stof(entry[0]), std::stof(entry[1]), std::stof(entry[2]));
					}
				}
			}
			if(size == 2)
				world->addWall(vertices[0].xz(), vertices[1].xz(), normal.xz());
			if(size == 3)
				world->addWall(vertices[0], vertices[1], normal);
		}else if(!line.substr(0,6).compare("floor ")){
			std::vector<Vec2> vertices(2);
			std::vector<std::string> entry1 = OBJ_Loader::split(data[1], ',');
			std::vector<std::string> entry2 = OBJ_Loader::split(data[2], ',');
			vertices[0] = Vec2(std::stof(entry1[0]), std::stof(entry1[1]));
			vertices[1] = Vec2(std::stof(entry2[0]), std::stof(entry2[1]));
			if(data.size() == 4){
				float height = std::stof(data[3]);
				world->addFloor(vertices[0], vertices[1], height);
			}else
				world->addFloor(vertices[0], vertices[1]);
		}else if(!line.substr(0,5).compare("ceil ")){
			if(data.size() == 4){
				std::vector<Vec2> vertices(2);
				float height = std::stof(data[3]);
				for(int i = 1; i <= 2; ++i){
					std::vector<std::string> entry = OBJ_Loader::split(data[i], ',');
					vertices[i-1] = Vec2(std::stof(entry[0]), std::stof(entry[1]));
				}
				world->addCeiling(vertices[0], vertices[1], height);
			}else if(data.size() == 5){
				std::vector<Vec3> vertices(4);
				for(int i = 1; i < data.size(); ++i){
					std::vector<std::string> entry = OBJ_Loader::split(data[i], ',');
					vertices[i-1] = Vec3(std::stof(entry[0]), std::stof(entry[1]), std::stof(entry[2]));
				}
				world->addCeiling(vertices[0], vertices[1], vertices[2], vertices[3]);
			}
		}else if(!line.substr(0,6).compare("stair ")){
			int inc = 3;
			float dir = 1;
			float length = 0.5f;
			float start = 0;
			float min = -1;
			float max = 1;
			float heightInc = 0.2f;
			float height = 0;
			float steps = std::stoi(data[1]);
			for(int i = 0; i < data.size(); ++i){
				std::vector<std::string> set = OBJ_Loader::split(data[i], '=');
				if(set[0] == "inc"){
					inc = std::stoi(set[1]);
				}else if(set[0] == "dir"){
					dir = std::stof(set[1]);
				}else if(set[0] == "length"){
					length = std::stof(set[1]);
				}else if(set[0] == "start"){
					start = std::stof(set[1]);
				}else if(set[0] == "min"){
					min = std::stof(set[1]);
				}else if(set[0] == "max"){
					max = std::stof(set[1]);
				}else if(set[0] == "heightInc"){
					heightInc = std::stof(set[1]);
				}else if(set[0] == "height"){
					height = std::stof(set[1]);
				}
			}
			int comp = (inc + 2) % 4;
			for(int i = 0; i < steps; ++i){
				float increment = i*dir*length + start;
				//Step
				Vec3 step1, step2, normal;
				step1[inc-1] = increment;
				step1[1] = i*heightInc+height;
				step1[comp-1] = min;
				step2[inc-1] = increment;
				step2[1] = (i+1)*heightInc+height;
				step2[comp-1] = max;
				normal[inc-1] = -dir;
				normal[1] = 0;
				normal[comp-1] = 0;
				world->addStep(step1, step2, normal);
				//Floor
				Vec2 floor1, floor2;
				int inc2 = (inc-1) / 2 + 1;
				int comp2 = inc2 % 2 + 1;
				floor1[inc-1] = increment-length;
				floor1[comp-1] = min;
				floor2[inc-1] = increment;
				floor2[comp-1] = max;
				world->addFloor(floor1, floor2, height+i*heightInc);
			}
		}
	}
	
	return world;
}

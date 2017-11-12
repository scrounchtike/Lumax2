
#ifndef DOOM_WORLD_LOADER_HPP
#define DOOM_WORLD_LOADER_HPP

#include <string>
#include <vector>
#include <fstream>
#include <map>

#include "DOOM_World.hpp"
#include "../rendering/Texture.hpp"

class DOOM_WorldLoader{
public:
	DOOM_WorldLoader();
	
	DOOM_World* loadLevel(std::string fileName);
private:
	std::map<std::string, Texture*> textures;
};

#endif

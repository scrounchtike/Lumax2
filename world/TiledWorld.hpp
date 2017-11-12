
#ifndef TILED_WORLD_HPP
#define TILED_WORLD_HPP

#include <vector>
#include <deque>
#include <mutex>
#include <thread>

#include "Terrain.hpp"
#include "../math.hpp"

//TODO: Should the Tiled World really deal with shade stuff?
#include "../resource_management/ResourceManager.hpp"
#include "../rendering/Camera.hpp"

class TiledWorld{
public:
	TiledWorld();

	void input();
	void update(Vec3 playerPosition);
	void render(Camera* camera);

	void loadChunk();
private:
	ResourceManager* manager;
	
	std::vector<Terrain*> terrains;
	int CHUNK_WIDTH;

	std::thread thread1;
	std::mutex mutex;
};

#endif

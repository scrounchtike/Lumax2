
#ifndef APP9_HPP
#define APP9_HPP

#include "../world/VoxelTerrain.hpp"
#include "../world/TiledWorld.hpp"

#include "../resource_management/ResourceManager.hpp"
#include "../rendering/Camera.hpp"
#include "../math.hpp"

class App9{
public:
	App9();

	void input();
	void update();
	void render();

	void cleanUp();
private:
	VoxelTerrain* voxel;
	ResourceManager* manager;

	Camera* camera;

	TiledWorld* world;
};

#endif

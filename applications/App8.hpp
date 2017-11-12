
#ifndef APP8_HPP
#define APP8_HPP

#include "../rendering/Shader.hpp"
#include "../rendering/Camera.hpp"
#include "../RL/Model3D.hpp"
#include "../rendering/Texture.hpp"

#include "../resource_management/ResourceManager.hpp"

class App8{
public:
	App8();

	void input();
	void update();
	void render();

	void cleanUp();
private:
	Camera* camera;

	ResourceManager* manager;
};

#endif

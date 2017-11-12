
#ifndef APP7_HPP
#define APP7_HPP

#include <vector>
#include <string>

#include "../math.hpp"
#include "../resource_management/ResourceManager.hpp"
#include "../rendering/Shader.hpp"
#include "../rendering/Camera.hpp"
#include "../rendering/Texture.hpp"
#include "../rendering/Text.hpp"

#include "../engine/Console.hpp"

#include "../DOOM/D_wall.hpp"
#include "../DOOM/DOOM_plane.hpp"
#include "../DOOM/DOOM_Player.hpp"
#include "../DOOM/DOOM_World.hpp"
#include "../DOOM/DOOM_World_Loader.hpp"
#include "../rendering/DebugRenderer.hpp"

#include "../engine/RenderingEngine.hpp"

class App7{
public:
	static void init();
	static void input();
	static void update();
	static void render();
	static void cleanUp();
private:
	//Callbacks
	static void pointCallback(std::string command);

	//Rendering Engine
	static RenderingEngine* engine;
	
	//static Camera camera;
	static DOOM_Player* player;
	
	static D_wall* wall1;
	static D_wall* wall2;
	static D_wall* wall3;
	static D_wall* wall4;
	
	static Shader* DOOM_shader;
	static Texture* wall_texture;
	static Texture* floor_texture;
	
	static DOOM_Plane* floor1;
	static DOOM_Plane* floor2;
	static DOOM_Plane* floor3;
	static DOOM_Plane* floor4;

	static DOOM_World* world;

	static Text* text;
	static Console* console;
	
	static bool inConsole;
};

#endif

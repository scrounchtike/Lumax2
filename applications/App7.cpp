
#include "App7.hpp"

#include "../main.hpp"

#include "../resource_management/TextureLoader.hpp"
#include "../RAL/Window.hpp"
#include "../OBJ_Loader.hpp"

#include "../rendering/Camera.hpp"

#include "../RAL/UsingGL.hpp"

//Camera  App7::camera(Vec3(0,0,0));
DOOM_Player* App7::player;

D_wall* App7::wall1;
D_wall* App7::wall2;
D_wall* App7::wall3;
D_wall* App7::wall4;

Shader* App7::DOOM_shader;

Texture* App7::wall_texture;
Texture* App7::floor_texture;

DOOM_Plane* App7::floor1;
DOOM_Plane* App7::floor2;
DOOM_Plane* App7::floor3;
DOOM_Plane* App7::floor4;

DOOM_World* App7::world;

Text*    App7::text;
Console* App7::console;

bool    App7::inConsole;

RenderingEngine* App7::engine;

void App7::init(){
	ResourceManager manager = ResourceManager("res");
	//floor_texture = new Texture("res/doom/floor/floor4_8.png");
	//floor_texture = new Texture(TextureLoader::loadTextureLIBPNG("res/doom/floor/floor4_8.png"));
	floor_texture = new Texture("res/doom/floor/floor4_8.png");
	wall_texture = new Texture("res/doom/stone/stone2.png");
	Texture* ceiling_texture = new Texture("res/doom/stone/stone3.png");

	Camera* camera = new Camera(window, Vec3(0, 0.6f, 0.5f));

	Text::initText();
	text = new Text("Hello World!", -0.8f, 0.6f);

	console = new Console();
	console->logCommand("point", pointCallback);

	engine = new RenderingEngine(camera);

	//test.png
	// Not working on Win32 (probably on Win64 too..)
	//TextureLoader::createTestTexture();
	
	player = new DOOM_Player(camera);
	world = new DOOM_World();
	
	DOOM_WorldLoader loader;
	world = loader.loadLevel("res/levels/level1.txt");
	
	DOOM_shader = new Shader("applications/app7/DOOM_Shader");
	DOOM_shader->addUniform("transform");
	DOOM_shader->addUniform("projection");
	DOOM_shader->addUniform("view");
}

void App7::input(){
	engine->input();
	if(inConsole)
		console->input();
	else
		player->input(world);
	
	if(!inConsole && wasKeyJustPressed(GLFW_KEY_C))
		inConsole = 1;
	if(inConsole && wasKeyJustPressed(GLFW_KEY_ESCAPE)){
		inConsole = 0;
		console->clear();
	}
}

void App7::update(){
	engine->update();
	if(inConsole)
		console->update();
	else
		player->update();
}

void App7::render(){
	engine->render();
	
	DOOM_shader->bind();
	DOOM_shader->setUniformMatrix("projection",
																player->getCamera()->getProjection().getHeadPointer());
	DOOM_shader->setUniformMatrix("view", player->getCamera()->getView().getHeadPointer());
	Mat4 transform = Mat4().initIdentity();
	DOOM_shader->setUniformMatrix("transform", transform.getHeadPointer());
	
	world->render();
	//player->render();

	//text->fullRender();
	if(inConsole)
		console->render();
}

void App7::cleanUp(){
	
}

void App7::pointCallback(std::string command){
	//std::cout << "Add a point to the scene" << std::endl;
	//Verify the format of the command
	std::vector<std::string> params = OBJ_Loader::split(command, ' ');
	std::vector<std::string> coords = OBJ_Loader::split(params[1], ',');
	Vec3 point = Vec3(std::stof(coords[0]), std::stof(coords[1]), std::stof(coords[2]));
	if(params.size() == 2){
		//Add a rendering engine!!
		engine->addPoint(point, Vec3(0,1,0));
	}else{
		std::cerr << "Unknown arguments passed to command point: " << command << std::endl;
		return;
	}
}

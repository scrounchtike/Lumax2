
#include "App9.hpp"

#include "../main.hpp"

#include "GLFW/glfw3.h"
#include "../RAL/Window.hpp"

App9::App9(){
	voxel = new VoxelTerrain();

	manager = new ResourceManager("res");
	
	Shader* voxelShader = manager->addShader("voxelShader", "applications/app9/shaderVoxel");
	voxelShader->addUniform("transform");
	voxelShader->addUniform("projection");
	voxelShader->addUniform("view");

	//world = new TiledWorld();

	camera = new Camera(window, Vec3(0,30,0));
	camera->setSpeed(0.03f);
}

void App9::input(){
  float speed = camera->getSpeed();
	Vec3 walk = camera->getWalk();
	Vec3 right = camera->getRight();
	Vec3 yAxis = camera->getYAxis();
	
	Vec3 posDelta = Vec3(0,0,0);
	if(wasKeyJustPressed(GLFW_KEY_W))
		posDelta += -(walk*speed);
	if(wasKeyJustPressed(GLFW_KEY_S))
		posDelta += (walk*speed);
	if(wasKeyJustPressed(GLFW_KEY_A))
		posDelta += -(right*speed);
	if(wasKeyJustPressed(GLFW_KEY_D))
		posDelta += (right*speed);

	//Up and down
	bool fly = true;
	if(fly){
		if(wasKeyJustPressed(GLFW_KEY_SPACE))
			posDelta += (yAxis*speed);
		if(wasKeyJustPressed(GLFW_KEY_LEFT_SHIFT))
			posDelta += -(yAxis*speed);
	}

	//world->checkCollisionWalls(camera->getPosition(), posDelta);
	//world->checkHeightOnFloor(camera->getPosition(), posDelta);
	voxel->checkCollisions(camera->getPosition(), posDelta);
	camera->input(posDelta);
	//world->input();
}

void App9::update(){
	camera->update();
	//world->update(camera->getPosition());
}

void App9::render(){
	Shader* voxelShader = manager->getShader("voxelShader");
	voxelShader->bind();

	voxelShader->setUniformMatrix("projection", camera->getProjection());
	voxelShader->setUniformMatrix("view", camera->getView());
	voxelShader->setUniformMatrix("transform", Mat4().initIdentity());
	
	voxel->render();

	//world->render(camera);
}

void App9::cleanUp(){
	
}


#include "DOOM_Player.hpp"

#include <iostream>

#include "../RAL/Window.hpp"
#include "GLFW/glfw3.h"

DOOM_Player::DOOM_Player(Camera* camera) : camera(camera){
	float vertices[8] = {0,1,0,0,1,0,1,1};
	int indices[6] = {0,1,2,2,3,0};
	float texCoords[8] = {0,0,0,1,1,1,1,0};
	model_player = new Model2D(vertices, 4, indices, 6, texCoords);

	gun = new Texture("res/test/resized_PISGA0.png");
	gun_firing = new Texture("res/doom/PISGB0.png");

	shader = new Shader("shaders/shaderPlayer");
	shader->addUniform("transform");
}

void DOOM_Player::input(DOOM_World* world){
	//TODO: Set player speed a constant in header
	float speed = 0.03f;
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
	bool fly = false;
	if(fly){
		if(wasKeyJustPressed(GLFW_KEY_SPACE))
			posDelta += (yAxis*speed);
		if(wasKeyJustPressed(GLFW_KEY_LEFT_SHIFT))
			posDelta += -(yAxis*speed);
	}

	world->checkCollisionWalls(camera->getPosition(), posDelta);
	world->checkHeightOnFloor(camera->getPosition(), posDelta);
	camera->input(posDelta);
}

void DOOM_Player::update(){
	camera->update();
}

void DOOM_Player::render(){
	glDisable(GL_DEPTH_TEST);
	
	shader->bind();
	shader->setUniformMatrix("transform", Mat4().initIdentity().getHeadPointer());
	
	gun->bind();
	model_player->fullRender();

	glEnable(GL_DEPTH_TEST);
}

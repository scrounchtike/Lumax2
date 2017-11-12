
#include "App8.hpp"

#include "../main.hpp"

#ifdef __unix
#include "OpenGL/gl3.h"
#elif (defined _WIN32 || defined _WIN64)
#include <GL/glew.h>
#endif

#include "../OBJ_Loader.hpp"
#include "../rendering/Rectangle.hpp"

App8::App8(){
	Rectangle::initRectangle();
	
	camera = new Camera(window, Vec3(0,0,0));

	manager = new ResourceManager("res");
	
	Shader* shaderNM = manager->addShader("shaderNM", "applications/app8/shaderNormalMap");
	shaderNM->addUniform("transform");
	shaderNM->addUniform("projection");
	shaderNM->addUniform("view");
	shaderNM->addUniform("diffuse");
	shaderNM->addUniform("normalMap");
	shaderNM->addUniform("cameraPos");

	manager->addTexture("bricks2.png");
	manager->addTexture("bricks2_normal.png");

	Rectangle::initRectangle(true);
	manager->addModel("plane", Rectangle::getModel());
}

void App8::input(){
	camera->input();
}

void App8::update(){
	camera->update();
}

void App8::render(){
	Shader* shaderNM = manager->getShader("shaderNM");
	shaderNM->bind();

	shaderNM->setUniformMatrix("projection", camera->getProjection());
	shaderNM->setUniformMatrix("view", camera->getView());
	shaderNM->setUniform3f("cameraPos", camera->getPosition());
	shaderNM->setUniform1i("normalMap", 1);
	shaderNM->setUniform1i("difuse", 0);

	glActiveTexture(GL_TEXTURE1);
	manager->getTexture("bricks2_normal.png")->bind();
	glActiveTexture(GL_TEXTURE0);
	manager->getTexture("bricks2.png")->bind();

	shaderNM->setUniformMatrix("transform", Mat4().initIdentity());
	
	manager->getModel3D("plane")->render();
}

void App8::cleanUp(){
	
}

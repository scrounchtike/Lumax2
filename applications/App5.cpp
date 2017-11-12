
#include "App5.hpp"

#include "../main.hpp"

#include <string>
#include "../rendering/stb_image.h"
#include "../rendering/Rectangle.hpp"

Shader* App5::shader;
Camera  App5::camera(nullptr, Vec3(0,0,0));

GLuint  App5::textureID;

void App5::init(){
	camera.setWindow(window);

	Cube::initCube();
	Rectangle::initRectangle();
	
	shader = new Shader("applications/app5/texture_shader");
	shader->addUniform("projection");
	shader->addUniform("view");
	shader->addUniform("transform");

	//Load texture
	std::string texturePath = "res/textures/test.png";
	int width, height, numComponents;
	unsigned char* image = stbi_load(texturePath.c_str(), &width, &height, &numComponents, STBI_rgb_alpha);

	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	stbi_image_free(image);
}

void App5::input(){
	camera.input();
}

void App5::update(){
	camera.update();
}

float App5::temp = 0;
void App5::render(){
	App5::temp += 0.01f;

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureID);
	
	shader->bind();
	shader->setUniformMatrix("projection", camera.getProjection().getHeadPointer());
	shader->setUniformMatrix("view", camera.getView().getHeadPointer());
	//Set transform uniform
	Mat4 translation = Mat4().initTranslation(0, 0, 5);
	Mat4 rotation = Mat4().initRotationY(App5::temp);
	Mat4 transform = translation.mul(rotation);
	Mat4 identity = Mat4().initIdentity();
	shader->setUniformMatrix("transform", transform.getHeadPointer());
	
	Cube::bindForRender();
	Cube::render();
	Cube::unbindForRender();

	glDisable(GL_CULL_FACE);

	//Rectangle::bindForRender();
	//Rectangle::render();
	//Rectangle::unbindForRender();
}

void App5::cleanUp(){
	delete shader;
}

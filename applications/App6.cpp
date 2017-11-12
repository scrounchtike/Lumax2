
#include "App6.hpp"

#include "../main.hpp"

#include <stdio.h>
#include <iostream>
#include <vector>
#include <sstream>

#include <cmath>

#include "../rendering/Rectangle.hpp"
#include "../OBJ_Loader.hpp"
#include "../baked_lighting/bake_light.hpp"

GLuint App6::vaoID;
GLuint App6::vboID;
GLuint App6::iboID;
GLuint App6::vbotID;
GLuint App6::vbonID;

int    App6::numVertices;

Shader*  App6::shader;
Shader*  App6::shader_light;
Camera*  App6::camera;
Texture* App6::texture;
Texture* App6::texture_gold;
GLuint   App6::baked_lighting;
GLuint   App6::baked_triangle;
GLuint   App6::baked_sphere;

BakedModel3D* App6::sphere;
BakedModel3D* App6::cube;
Model3D* App6::triangle;
Vec3 barycentric(Vec3 P, Vec3 A, Vec3 B, Vec3 C);

void App6::init(){
	Rectangle::initRectangle();
	
	texture = new Texture("res/textures/test.png");
	texture_gold = new Texture("res/textures/gold.png");
	
	std::string filePath = "res/models/cube.obj";

	RawModel model = OBJ_Loader::loadModel(filePath);
	//cube = new Model3D(model.vertices, model.numVertices, model.texCoords, model.normals);
	RawModel model2 = OBJ_Loader::loadModel("res/models/sphere.obj");
	//sphere = new Model3D(model2.vertices, model2.numVertices, model2.texCoords, model2.normals);

	float vertices_t[9] = {2,1,0,0,-1,1,-3,3,1};
	float texCoords_t[6] = {0,0.5f,1,1,1,0};
	Vec3 A_ = Vec3(-3,3,1) - Vec3(2,1,0);
	Vec3 B_ = Vec3(0,-1,1) - Vec3(2,1,0);
	Vec3 normal = cross(B_, A_);
	normal = normal.normalize();
	float normals_t[9] = {normal.x, normal.y, normal.z, normal.x, normal.y, normal.z, normal.x, normal.y, normal.z};

	triangle = new Model3D(vertices_t, 3, texCoords_t, normals_t);

	//Bake lighting onto triangle
	float smin = 0, smax = 1;
	float tmin = 0, tmax = 1;
	for(int i = 0; i < 3; ++i){
		if(texCoords_t[i*2] < smin)
			smin = texCoords_t[i*2];
		if(texCoords_t[i*2] > smax)
			smax = texCoords_t[i*2];
		if(texCoords_t[i*2+1] < tmin)
			tmin = texCoords_t[i*2+1];
		if(texCoords_t[i*2+1] > tmax)
			tmax = texCoords_t[i*2+1];
	}

	//Vec3 verts[3] = {Vec3(-1,0,0), Vec3(-1,1,0), Vec3(0,0,0)};
	//Vec3 verts[3] = {Vec3(0,1,0), Vec3(0,0,0), Vec3(-1,1,0)};
	Vec3 verts[4] = {Vec3(2,1,0), Vec3(0,-1,1), Vec3(-2,-2,0), Vec3(-3,3,1)};
	//                00             01          10
	const int SIZE = 16;
	unsigned char data[SIZE * SIZE * 3];
	float step = 1.0f / (float)SIZE;

	Vec3 light_pos = Vec3(0,0,-1);
	Vec3 light_color = Vec3(1,1,1);
	Vec3 pos;
	float s_dist, t_dist;
	Vec3 s_vec = verts[3] - verts[0];
	//Vec3 s_vec = Vec3(1,0,0) - Vec3(0,0,0);
	s_dist = sqrt(dot(s_vec, s_vec));
	Vec3 t_vec = verts[1] - verts[0];
	//Vec3 t_vec = Vec3(0,1,0) - Vec3(0,0,0);
	t_dist = sqrt(dot(t_vec, t_vec));
	float s, t;
	s = t = 0.0f;
	for(int i = 0; i < SIZE; ++i){
		for(int j = 0; j < SIZE; ++j){
			pos.x = s_dist * s;
			pos.y = t_dist * t;
			pos.z = 0;
			
			//Vec3 brightness = Vec3(0,0,0);

			//Transform light pos to texCoord coordinates
			Vec3 light_p = light_pos + verts[0];
			
			//Perform transformations on texel
			//none

			float exponent = 0.5f;
			float linear = 0.05f;
			float constant = 0.01f;
			
			float intensity = 0.4f;
			Vec3 light = pos - light_p;
			float distance = light.getLength();
			light = light.normalize();
			float dot1 = dot(-light, Vec3(0,0,-1));
			Vec3 brightness = Vec3(0.0f,0.0f,0.0f);
			if(dot1 > 0){
				Vec3 factor = light_color * dot1 * intensity;
				//attenuation
				float attenuation = (exponent*distance*distance)+(linear*distance)+constant;
				brightness += factor / attenuation;
			}
			
			brightness += Vec3(0.3f,0.3f,0.3f);
			
			if(brightness.x > 1.0f)
				brightness.x = 1.0f;
			if(brightness.y > 1.0f)
				brightness.y = 1.0f;
			if(brightness.z > 1.0f)
				brightness.z = 1.0f;
			
			//std::cout << "pos = " << pos.x << " " << pos.y << std::endl;
			//d = dot(pos,pos) * 0.8f;
			//if(d < 1.0f)
			//d = 1.0f;
			//tmp = 1.0f / d;
			
			data[(i * SIZE + j) * 3 + 0] = (unsigned char)(255.0f * brightness.x);
			data[(i * SIZE + j) * 3 + 1] = (unsigned char)(255.0f * brightness.y);
			data[(i * SIZE + j) * 3 + 2] = (unsigned char)(255.0f * brightness.z);

			s += step;
		}
		s = 0.0f;
		t += step;
	}

	BakeLight bake;
  //baked_triangle = bake.bake(verts[0], verts[1], verts[3], normal);
	//std::vector<float> baked_texs;
	//baked_sphere = bake.bake_model(model2, baked_texs);

	//std::cout << "NUM BAKED TEXS = " << baked_texs.size() << std::endl;
	//float bakedTexCoords[baked_texs.size()];
	//for(int i = 0; i < baked_texs.size(); ++i)
	//	bakedTexCoords[i] = baked_texs[i];
	//sphere = new BakedModel3D(model2.vertices, model2.numVertices, model2.texCoords, model2.normals, bakedTexCoords);

	std::vector<float> baked_texs;
	baked_sphere = bake.bake_model(model, baked_texs);
	float* bakedTexCoords = &baked_texs[0];
	//for(int i = 0; i < baked_texs.size(); ++i)
	//	bakedTexCoords[i] = baked_texs[i];
	cube = new BakedModel3D(model.vertices, model.numVertices, model.texCoords, model.normals, bakedTexCoords);
	
	numVertices = 3;

	glGenVertexArrays(1, &vaoID);
	glBindVertexArray(vaoID);

	glGenBuffers(1, &vboID);
	glGenBuffers(1, &iboID);
	glGenBuffers(1, &vbotID);
	glGenBuffers(1, &vbonID);

	glBindBuffer(GL_ARRAY_BUFFER, vboID);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(float)*model.numVertices*3, vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, false, 0, 0);
	
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iboID);
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int)*36, &indices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, vbotID);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(float)*model.numVertices*2, texCoords, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 2, GL_FLOAT, false, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, vbonID);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(float)*model.numVertices*3, normals, GL_STATIC_DRAW);
	glVertexAttribPointer(2, 3, GL_FLOAT, false, 0, 0);
	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	camera = new Camera(window, Vec3(0,0,0));
	
	shader = new Shader("applications/app6/bake_shader");
	shader->addUniform("projection");
	shader->addUniform("view");
	shader->addUniform("transform");
	shader->addUniform("eyePos");
	shader->addUniform("sample");
	shader->addUniform("baked_light");

	shader_light = new Shader("applications/app6/lighting_shader");
	shader_light->addUniform("projection");
	shader_light->addUniform("view");
	shader_light->addUniform("transform");
	shader_light->addUniform("eyePos");

	//bakeLight();
}

void App6::bakeLight(){
	//Baked lighting
	Vec3 light_pos = Vec3(0,0,-1);
	Vec3 light_color = Vec3(1,1,1);

	//Surface
	Vec3 vertices[4] = {Vec3(-1,1,0),Vec3(-1,-1,0),Vec3(1,-1,0),Vec3(1,1,0)};
	float s_dist, t_dist;
	Vec3 s_vec = vertices[3] - vertices[0];
	s_dist = sqrt(dot(s_vec, s_vec));
	Vec3 t_vec = vertices[1] - vertices[0];
	t_dist = sqrt(dot(t_vec, t_vec));
	
	const int LIGHTMAP_SIZE = 16;
	unsigned char data[LIGHTMAP_SIZE * LIGHTMAP_SIZE * 3];
	Vec3 pos;
	float step, s, t;
	step = 1.0f / (float)LIGHTMAP_SIZE;
	
	glGenTextures(1, &baked_lighting);
	
	s = t = 0.0f;
	for(int i = 0; i < LIGHTMAP_SIZE; ++i){
		for(int j = 0; j < LIGHTMAP_SIZE; ++j){
			float d, tmp;
			pos.x = s_dist * s;
			pos.y = t_dist * t;
			pos.z = 0;
			
			pos -= Vec3(1,1,0);
			
			//Perform transformations on texel
			//none

			float exponent = 0.5f;
			float linear = 0.05f;
			float constant = 0.01f;
			
			float intensity = 0.4f;
			Vec3 light = pos - light_pos;
			float distance = light.getLength();
			light = light.normalize();
			float dot1 = dot(-light, Vec3(0,0,-1));
			Vec3 brightness = Vec3(0.0f,0.0f,0.0f);
			if(dot1 > 0){
				Vec3 factor = light_color * dot1 * intensity;
				//attenuation
				float attenuation = (exponent*distance*distance)+(linear*distance)+constant;
				brightness += factor / attenuation;
			}

			brightness += Vec3(0.3f,0.3f,0.3f);
			
			if(brightness.x > 1.0f)
				brightness.x = 1.0f;
			if(brightness.y > 1.0f)
				brightness.y = 1.0f;
			if(brightness.z > 1.0f)
				brightness.z = 1.0f;
			
			//std::cout << "pos = " << pos.x << " " << pos.y << std::endl;
			//d = dot(pos,pos) * 0.8f;
			//if(d < 1.0f)
			//d = 1.0f;
			//tmp = 1.0f / d;
			
			data[(i * LIGHTMAP_SIZE + j) * 3 + 0] = (unsigned char)(255.0f * brightness.x);
			data[(i * LIGHTMAP_SIZE + j) * 3 + 1] = (unsigned char)(255.0f * brightness.y);
			data[(i * LIGHTMAP_SIZE + j) * 3 + 2] = (unsigned char)(255.0f * brightness.z);
			
			s += step;
		}
		t += step;
		s = 0.0f;
	}
	
	//Create OpenGL texture;
	glBindTexture(GL_TEXTURE_2D, baked_lighting);
	//glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	//glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, LIGHTMAP_SIZE, LIGHTMAP_SIZE, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	glBindTexture(GL_TEXTURE_2D, baked_lighting);
}

void App6::input(){
	camera->input();
}

void App6::update(){
	camera->update();
}

void App6::render(){
	//glDisable(GL_CULL_FACE);

	shader->bind();

	//glActiveTexture(GL_TEXTURE1);
	//glBindTexture(GL_TEXTURE_2D, baked_lighting);
	//shader->setUniform1i("baked_light", 1);
	//glActiveTexture(GL_TEXTURE0);
	//texture->bind();
	//shader->setUniform1i("sample", 0);

	//shader->setUniformMatrix("projection", camera->getProjection().getHeadPointer());
	//shader->setUniformMatrix("view", camera->getView().getHeadPointer());
	//Vec3 cam = camera->getPosition();
	//shader->setUniform3f("eyePos", cam.x, cam.y, cam.z);
	//shader->setUniformMatrix("transform", Mat4().initIdentity().getHeadPointer());
	
	//Rectangle::bindForRender(false);
	//Rectangle::render();
	//Rectangle::unbindForRender(false);

	//Mat4 transform_plane = Mat4().initTranslation(-3, 0, 0);
	//shader_light->setUniformMatrix("transform", transform_plane.getHeadPointer());
	//Rectangle::bindForRender(true);
	//Rectangle::render();
	//Rectangle::unbindForRender(true);



	
	//Render triangles
	
	shader->bind();
	shader->setUniformMatrix("projection", camera->getProjection().getHeadPointer());
	shader->setUniformMatrix("view", camera->getView().getHeadPointer());
	Vec3 cam = camera->getPosition();
	shader->setUniform3f("eyePos", cam.x, cam.y, cam.z);
	//shader->setUniformMatrix("transform", Mat4().initTranslation(-4, 0, 0).getHeadPointer());

	
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, baked_sphere);
	shader->setUniform1i("baked_light", 1);
	glActiveTexture(GL_TEXTURE0);
	texture_gold->bind();
	shader->setUniform1i("sample", 0);
	
	Mat4 sphere_transform = Mat4().initTranslation(-3, 0, 0);
	Mat4 sphere_scale = Mat4().initScale(0.5f, 0.5f, 0.5f);
	sphere_transform = sphere_transform.mul(sphere_scale);
	shader->setUniformMatrix("transform", sphere_transform.getHeadPointer());

	//sphere->fullRender();
	cube->render();
	
	
	shader_light->bind();
	shader_light->setUniformMatrix("projection", camera->getProjection().getHeadPointer());
	shader_light->setUniformMatrix("view", camera->getView().getHeadPointer());
	shader_light->setUniform3f("eyePos", cam.x, cam.y, cam.z);

	glActiveTexture(GL_TEXTURE0);
	texture->bind();
	shader_light->setUniformMatrix("transform", Mat4().initIdentity().getHeadPointer());
	

	//Render sphere
	texture_gold->bind();

	Mat4 dragon_transform = Mat4().initTranslation(0, 0, 0);
	Mat4 dragon_scale = Mat4().initScale(0.5f, 0.5f, 0.5f);
	dragon_transform = dragon_transform.mul(dragon_scale);
	shader_light->setUniformMatrix("transform", dragon_transform.getHeadPointer());

	//sphere->fullRender();
	cube->render();

	//Test Plane for baked_sphere texture
	glBindTexture(GL_TEXTURE_2D, baked_sphere);
	Mat4 transform_plane = Mat4().initTranslation(3, 0, 0);
	shader_light->setUniformMatrix("transform", transform_plane.getHeadPointer());
	//Rectangle::fullRender(false);

	texture->bind();
	//Render cube
	Mat4 transform = Mat4().initTranslation(0, 0, 5);
	shader_light->setUniformMatrix("transform", transform.getHeadPointer());

	//cube->fullRender();
}

void App6::cleanUp(){
	glDeleteBuffers(1, &vboID);
	glDeleteBuffers(1, &iboID);
	glDeleteBuffers(1, &vbotID);
	glDeleteBuffers(1, &vbonID);
	glDeleteVertexArrays(1, &vaoID);

	delete shader;
	delete camera;
}

Vec3 barycentric(Vec3 P, Vec3 A, Vec3 B, Vec3 C){
	Vec3 v0 = B - A;
	Vec3 v1 = C - A;
	Vec3 v2 = P - A;
	float d00 = dot(v0, v0);
	float d01 = dot(v0, v1);
	float d11 = dot(v1, v1);
	float d20 = dot(v2, v0);
	float d21 = dot(v2, v1);
	float denom = d00 * d11 - d01 * d01;
	Vec3 result;
	result.y = (d11 * d20 - d01 * d21) / denom;
	result.z = (d00 * d21 - d01 * d20) / denom;
	result.x = 1.0f - result.y - result.z;
	return result;
}

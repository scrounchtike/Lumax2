
#include "radiosity.hpp"

#include "../main.hpp"

#include <stdlib.h>
#include <iostream>

#include "../rendering/DebugRenderer.hpp"
#include "polygon_clip.hpp"
#include "Polygon_Fill.hpp"
#include "Hemicube.hpp"

Shader* Radiosity::shader;

GLuint Radiosity::vaoID_light;
GLuint Radiosity::vboID_light;
GLuint Radiosity::vaoID_wall;
GLuint Radiosity::vboID_wall;
GLuint Radiosity::iboID_wall;

Camera Radiosity::camera(nullptr, Vec3(0,0,3));

Mat4   Radiosity::transform_light;

void Radiosity::initRadiosity(){
	camera.setWindow(window);

	/* TEST ON POLYGON CLIPPING */
	PolygonClass input;
	//input.vertices.push_back(Vec4(-0.5f,-4,0,0));
	input.vertices.push_back(Vec4(0,-0.5f,0,0));
	input.vertices.push_back(Vec4(0,4,0,0));
	input.vertices.push_back(Vec4(-2,4,0,0));
	input.vertices.push_back(Vec4(-3,1.5f,0,0));
	input.vertices.push_back(Vec4(-3,-0.5f,0,0));

	//PolygonClip clip;
	//Polygon output = clip.clipPolygon(input);
	//std::cout << "results" << std::endl;
	//for(int i = 0; i < output.vertices.size(); ++i){
	//	Vec4 vertex = output.vertices[i];
	//	std::cout << "VERTEX: " << i << " = " << vertex.x << " " << vertex.y << " " << vertex.z << " " << vertex.w << std::endl;
	//}

	PolygonClass toFill;
	//Vertices have to be in clockwise order
	toFill.vertices.push_back(Vec4(10, 16.65,0,0));
	toFill.vertices.push_back(Vec4(10,0.02,0,0));
	toFill.vertices.push_back(Vec4(0,2.52,0,0));
	toFill.vertices.push_back(Vec4(0,19.15,0,0));

	//toFill.vertices.push_back(Vec4(10, 16.65, 0, 0));
	//toFill.vertices.push_back(Vec4(10, 0.02, 0, 0));
	//toFill.vertices.push_back(Vec4(0, 2.52, 0, 0));
	//toFill.vertices.push_back(Vec4(0, 19.15, 0, 0));
	
	//PolygonFill fill;
	//fill.fillPolygon(toFill);
	//int canvas[20][20];
	//for(int i = 0; i < 20; ++i)
	//for(int j = 0; j < 20; ++j)
	//canvas[i][j] = 0.0;
	
	//fill.fillCanvas(canvas);
	
	
	//Init shader
	shader = new Shader("radiosity/radiosity_shader");
	shader->addUniform("projection");
	shader->addUniform("view");
	shader->addUniform("transform");
	shader->addUniform("color");

	//Light model
	float vertices_light[18] = {-0.5f, -0.5f, 0.0f, 0.5f, -0.5f, 0.0f, 0.5f, 0.5f, 0.0f,
															0.5f, 0.5f, 0.0f, -0.5f, 0.5f, 0.0f, -0.5f, -0.5f, 0.0f
	};
	glGenBuffers(1, &vboID_light);
	glBindBuffer(GL_ARRAY_BUFFER, vboID_light);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float)*18, vertices_light, GL_STATIC_DRAW);
	
	glGenVertexArrays(1, &vaoID_light);
	glBindVertexArray(vaoID_light);

	glVertexAttribPointer(0, 3, GL_FLOAT, false, 0, 0);
	
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	//Init matrices
	transform_light.initTranslation(0, 0, 4);
	transform_light.mul(Mat4().initRotationY(0.0));
	transform_light.mul(Mat4().initScale(0.5f, 0.5f, 0.5f));

	//PolygonFill fill;
	//fill.fillPolygon(toFill);
	//int canvas[20][20];
	//for(int i = 0; i < 20; ++i)
		//for(int j = 0; j < 20; ++j)
			//canvas[i][j] = 0.0;

	//fill.fillCanvas(canvas);
	//float size = 1.0f/20.0f;
	//float x = 2;
	//float y = 9;
	//for(int y = 0; y < 20; ++y){
		//for(int x = 0; x < 20; ++x){
			//if(!canvas[y][x])
				//continue;
			//DebugRenderer::addQuad(Vec3(-1+size+(x*size*2),-1+size+(y*size*2),2), Vec3(size,size,0.1f), Vec3(1,1,0));
	//}
	//}

	//Debug Rendering
	//DebugRenderer::addPoint(Vec3(5.0/10.0-1.0,5.0/10.0-1.0,2), Vec3(0,1,0));
	//DebugRenderer::addPoint(Vec3(5.0/10.0-1.0,15.0/10.0-1.0,2), Vec3(0,1,0));
	//DebugRenderer::addPoint(Vec3(0.0f,0.0f,2), Vec3(0,1,0));

	//Eye of hemicube
	DebugRenderer::addPoint(Vec3(0,0,3), Vec3(1,1,1));

	//Render Hemicube 20x20 grid for projection
	for(int i = 0; i < 101; ++i){
		float pos = (float)2.0*i/100.0f - 1.0;
		DebugRenderer::addLine(Line(Vec3(pos,-1,2), Vec3(pos,1,2), Vec3(0.5f,0.5f,0.5f)));
		DebugRenderer::addLine(Line(Vec3(-1,pos,2), Vec3(1,pos,2), Vec3(0.5f,0.5f,0.5f)));

		DebugRenderer::addLine(Vec3(pos,1,3), Vec3(pos,1,2), Vec3(0.5f,0.5f,0.5f));
		DebugRenderer::addLine(Vec3(pos,-1,3), Vec3(pos,-1,2), Vec3(0.5f,0.5f,0.5f));
		DebugRenderer::addLine(Vec3(1,pos,3), Vec3(1,pos,2), Vec3(0.5f,0.5f,0.5f));
		DebugRenderer::addLine(Vec3(-1,pos,3), Vec3(-1,pos,2), Vec3(0.5f,0.5f,0.5f));
	}
	//Render all 4 rectangular sides
	for(int i = 0; i < 51; ++i){
		float pos = (float)2.0*i/100.0f - 1.0;
		DebugRenderer::addLine(Vec3(-1,1,pos+3), Vec3(1,1,pos+3), Vec3(0.5f,0.5f,0.5f));
		DebugRenderer::addLine(Vec3(-1,-1,pos+3), Vec3(1,-1,pos+3), Vec3(0.5f,0.5f,0.5f));
		DebugRenderer::addLine(Vec3(1,1,pos+3), Vec3(1,-1,pos+3), Vec3(0.5f,0.5f,0.5f));
		DebugRenderer::addLine(Vec3(-1,1,pos+3), Vec3(-1,-1,pos+3), Vec3(0.5f,0.5f,0.5f));
	}

	float size = 12.0/20.0;
	Vec3 start = Vec3(-6+size/2.0,-6+size/2.0,-5);
	for(int i = 0; i < 20; ++i){
		for(int j = 0; j < 20; ++j){
			float color = (float)rand() / (float)RAND_MAX;
			DebugRenderer::addQuad(start+Vec3(i*size, j*size, 0), Vec3(size/2.0,size/2.0,1), Vec3(color,color,color));
		}
	}

	//Polygon into hemicube
	Vec3 eye_pos = Vec3(0,0,3);
	PolygonClass polygon;
	//polygon.vertices.push_back(Vec4(1.5f+3,2,0,0));
	//polygon.vertices.push_back(Vec4(-0.5f+3,2,0,0));
	//polygon.vertices.push_back(Vec4(-0.5f+3,0,0,0));
	//polygon.vertices.push_back(Vec4(2+3,0,0,0));

	polygon.vertices.push_back(Vec4(-6, -6, -5, 0));
	polygon.vertices.push_back(Vec4(-6, -6+size, -5, 0));
	polygon.vertices.push_back(Vec4(-6+size, -6+size, -5, 0));
	polygon.vertices.push_back(Vec4(-6+size, -6, -5, 0));

	//polygon.vertices.push_back(Vec4(-4,-3,3,1));
	//polygon.vertices.push_back(Vec4(-4,2,3,1));
	//polygon.vertices.push_back(Vec4(-4,2,1.5f,1));
	//polygon.vertices.push_back(Vec4(-4,-2,1,1));
	
	for(int i = 0; i < polygon.vertices.size(); ++i)
		DebugRenderer::addLine(eye_pos, polygon.vertices[i].xyz(), Vec3(1,1,0));

	Hemicube hemicube;
	hemicube.transformPolygon(polygon);
}

void Radiosity::cleanUpRadiosity(){
	glDeleteVertexArrays(1, &vaoID_light);
	glDeleteBuffers(1, &vboID_light);
}

void Radiosity::input(){
	camera.input();
}

void Radiosity::update(){
	camera.update();
}

void Radiosity::render(){
	glDisable(GL_CULL_FACE);

	//Bind shader and set uniforms
	shader->bind();
	shader->setUniform3f("color", 1.0, 0.0, 0.0);
	shader->setUniformMatrix("transform", transform_light.getHeadPointer());
	shader->setUniformMatrix("projection", camera.getProjection().getHeadPointer());
	shader->setUniformMatrix("view", camera.getView().getHeadPointer());

	//Render light model
	glBindVertexArray(vaoID_light);
	glEnableVertexAttribArray(0);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glDisableVertexAttribArray(0);
	glBindVertexArray(0);

	//Debug Rendering
	DebugRenderer::render(&camera);
}

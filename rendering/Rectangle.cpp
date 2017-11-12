
#include "Rectangle.hpp"

Model3D* Rectangle::model;

void Rectangle::initRectangle(bool calcTangents){
	//float vertices[18] = {-1,-1,0,1,-1,0,1,1,0,1,1,0-1,1,0,-1,-1,0};
	float vertices[12] = {-1,1,0,-1,-1,0,1,-1,0,1,1,0};
	float texCoords[8] = {0,0,0,1,1,1,1,0};
	float normals[12] = {0,0,-1,0,0,-1,0,0,-1,0,0,-1};
	int indices[6] = {2,1,0,0,3,2};

	model = new Model3D(vertices, 12, indices, 6, texCoords, normals, true);
}

Model3D* Rectangle::getModel(){
	return model;
}


#include "Terrain.hpp"

Terrain::Terrain(Vec3 position, int LOD) : position(position), LOD(LOD){
	generate();
}

void Terrain::generate(){
	createMesh();
	initGlRenderData();
}

void Terrain::regenerate(){
	createMesh();
}

void Terrain::createMesh(){
	vertices.clear();
	indices.clear();
	float SIZE = 16;
	vertices.push_back(Vec3(0,0,0));
	vertices.push_back(Vec3(SIZE,0,0));
	vertices.push_back(Vec3(SIZE,0,SIZE));
	vertices.push_back(Vec3(0,0,SIZE));
	indices.push_back(0);
	indices.push_back(1);
	indices.push_back(2);
	indices.push_back(2);
	indices.push_back(3);
	indices.push_back(0);
}

void Terrain::initGlRenderData(){
	glGenVertexArrays(1, &vaoID);
	glBindVertexArray(vaoID);

	glGenBuffers(1, &vboID);
	glBindBuffer(GL_ARRAY_BUFFER, vboID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float)*vertices.size()*3, &vertices[0].x, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, false, 0, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenBuffers(1, &iboID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iboID);
	numIndices = indices.size();
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int)*indices.size(), &indices[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	glBindVertexArray(0);
}

void Terrain::input(){
	
}

void Terrain::update(){
	
}

void Terrain::render(){
	glBindVertexArray(vaoID);

	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iboID);
	glDrawElements(GL_TRIANGLES, numIndices, GL_UNSIGNED_INT, 0);

	glDisableVertexAttribArray(0);
	
	glBindVertexArray(0);
}

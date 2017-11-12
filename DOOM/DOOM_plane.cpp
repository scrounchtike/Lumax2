
#include "DOOM_plane.hpp"

#include <cmath>

DOOM_Plane::DOOM_Plane(Vec3 pos1, Vec3 pos2, Vec3 pos3, Vec3 pos4, Vec3 normal, Vec3 tangent) : normal(normal){
	//Do we need to invert vertices?
	normal.normalize();
	Vec3 top_direction = cross(tangent, normal);
	//top_direction.normalize();
	Vec3 top = pos4 - pos1;
	float* vertices;
	if(dot(top, top_direction) > 0){
		float verts[12] = {pos1.x, pos1.y, pos1.z,
											 pos2.x, pos2.y, pos2.z,
											 pos3.x, pos3.y, pos3.z,
											 pos4.x, pos4.y, pos4.z};
		start = pos1;
		end = pos3;
		vertices = verts;
	}else{
		float verts[12] = {pos4.x, pos4.y, pos4.z,
											 pos3.x, pos3.y, pos3.z,
											 pos2.x, pos2.y, pos2.z,
											 pos1.x, pos1.y, pos1.z};
		start = pos4;
		end = pos2;
		vertices = verts;
	}
	int indices[6] = {0,1,2,2,3,0};
	Vec3 s_vec = pos4 - pos1;
	Vec3 t_vec = pos2 - pos1;
	float s_diff = sqrt(dot(s_vec, s_vec));
	float t_diff = sqrt(dot(t_vec, t_vec));
	float texCoords[8] = {0,0,0,t_diff,s_diff,t_diff,s_diff,0};
	float normals[12] = {normal.x, normal.y, normal.z,
											 normal.x, normal.y, normal.z,
											 normal.x, normal.y, normal.z,
											 normal.x, normal.y, normal.z};

	model = new Model3D(vertices, 4, indices, 6, texCoords, normals);
}

void DOOM_Plane::bindForRender(){
	model->bindForRender();
}

void DOOM_Plane::render(){
	model->render();
}

void DOOM_Plane::unbindForRender(){
	model->unbindForRender();
}


void DOOM_Plane::fullRender(){
	model->fullRender();
}

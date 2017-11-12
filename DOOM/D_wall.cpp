
#include "D_wall.hpp"

D_wall::D_wall(Vec2 pos1, Vec2 pos2, Vec2 normal){
	float vertices[12] = {pos1.x, 1, pos1.y,
												pos1.x, 0, pos1.y,
												pos2.x, 0, pos2.y,
												pos2.x, 1, pos2.y,};
	int indices[6] =     {2,1,0,0,3,2};
	float texCoords[8] = {0,0,0,1,1,1,1,0};
	float normals[12] =  {normal.x, 0, normal.y,
												normal.x, 0, normal.y,
												normal.x, 0, normal.y,
												normal.x, 0, normal.y};

	wall_model = new Model3D(vertices, 4, indices, 6, texCoords, normals);
	
	
}

void D_wall::bindForRender(){
	wall_model->bindForRender();
}

void D_wall::render(){
	wall_model->render();
}

void D_wall::unbindForRender(){
	wall_model->unbindForRender();
}

void D_wall::fullRender(){
	wall_model->fullRender();
}

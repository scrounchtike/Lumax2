
#include "VoxelTerrain.hpp"

#include "fBmModule.hpp"

VoxelTerrain::VoxelTerrain(){
	fBmModule module = fBmModule(6, 0.6f, 10);
	float** heights = module.generate(16);
	
	height = 128;
	size = 16;
	//Init blocks according to fBm noise
	blocks = new unsigned int**[height];
	for(int i = 0; i < height; ++i){
		blocks[i] = new unsigned int*[size];
		for(int j = 0; j < size; ++j){
			blocks[i][j] = new unsigned int[size];
			//initialization to 0
			for(int k = 0; k < size; ++k)
				blocks[i][j][k] = 0;
		}
	}
	int offset = 20;
	for(int h = 0; h < height; ++h){
		for(int x = 0; x < size; ++x){
			for(int z = 0; z < size; ++z){
				if(h < (heights[x][z]+offset))
					blocks[h][x][z] = 1;
			}
		}
	}
	generateRenderData();
}

void VoxelTerrain::generateRenderData(){
	int index = 0;
	std::vector<Vec3> vertices;
	std::vector<Vec3> normals;
	std::vector<float> AO;
	std::vector<int> indices;
	unsigned int next;
	for(int h = 0; h < height; ++h){
		for(int x = 0; x < size; ++x){
			for(int z = 0; z < size; ++z){
				if(!blocks[h][x][z])
					continue;
			  if(h+1 >= height || !blocks[h+1][x][z]){
					vertices.push_back(Vec3(x,h+1,z));
					AO.push_back(calcAO(testForBlock(h+1,x,z-1), testForBlock(h+1,x-1,z), testForBlock(h+1,x-1,z-1)));
					vertices.push_back(Vec3(x,h+1,z+1));
					AO.push_back(calcAO(testForBlock(h+1,x-1,z), testForBlock(h+1,x,z+1), testForBlock(h+1,x-1,z+1)));
					vertices.push_back(Vec3(x+1,h+1,z+1));
					AO.push_back(calcAO(testForBlock(h+1,x+1,z), testForBlock(h+1,x,z+1), testForBlock(h+1,x+1,z+1)));
					vertices.push_back(Vec3(x+1,h+1,z));
					AO.push_back(calcAO(testForBlock(h+1,x+1,z), testForBlock(h+1,x,z-1), testForBlock(h+1,x+1,z-1)));
					addIndices(indices, index);
					addNormals(normals, Vec3(0,1,0));
				}
				//TODO: Implement ambient occlusion for faces pointing down.
				if(h-1 < 0 || !blocks[h-1][x][z]){
					vertices.push_back(Vec3(x+1,h,z));
					vertices.push_back(Vec3(x+1,h,z+1));
					vertices.push_back(Vec3(x,h,z+1));
					vertices.push_back(Vec3(x,h,z));
					addAO(AO, 3);
					addIndices(indices, index);
					addNormals(normals, Vec3(0,-1,0));
				}
				if(x+1 >= size || !blocks[h][x+1][z]){
					vertices.push_back(Vec3(x+1,h,z+1));
					AO.push_back(calcAO(testForBlock(h,x+1,z+1), testForBlock(h-1,x+1,z), testForBlock(h-1,x+1,z+1)));
					vertices.push_back(Vec3(x+1,h,z));
					AO.push_back(calcAO(testForBlock(h,x+1,z-1), testForBlock(h-1,x+1,z), testForBlock(h-1,x+1,z-1)));
					vertices.push_back(Vec3(x+1,h+1,z));
					AO.push_back(calcAO(testForBlock(h,x+1,z-1), testForBlock(h+1,x+1,z), testForBlock(h+1,x+1,z-1)));
					vertices.push_back(Vec3(x+1,h+1,z+1));
					AO.push_back(calcAO(testForBlock(h,x+1,z+1), testForBlock(h+1,x+1,z), testForBlock(h+1,x+1,z+1)));
					addIndices(indices, index);
					addNormals(normals, Vec3(1,0,0));
				}
				if(x-1 < 0 || !blocks[h][x-1][z]){
					vertices.push_back(Vec3(x,h,z));
					AO.push_back(calcAO(testForBlock(h,x-1,z-1), testForBlock(h-1,x-1,z), testForBlock(h-1,x-1,z-1)));
					vertices.push_back(Vec3(x,h,z+1));
					AO.push_back(calcAO(testForBlock(h,x-1,z+1), testForBlock(h-1,x-1,z), testForBlock(h-1,x-1,z+1)));
					vertices.push_back(Vec3(x,h+1,z+1));
					AO.push_back(calcAO(testForBlock(h,x-1,z+1), testForBlock(h+1,x-1,z), testForBlock(h+1,x-1,z+1)));
					vertices.push_back(Vec3(x,h+1,z));
					AO.push_back(calcAO(testForBlock(h,x-1,z-1), testForBlock(h+1,x-1,z), testForBlock(h+1,x-1,z-1)));
					addIndices(indices, index);
					addNormals(normals, Vec3(-1,0,0));
				}
				if(z+1 >= size || !blocks[h][x][z+1]){
					vertices.push_back(Vec3(x,h,z+1));
					AO.push_back(calcAO(testForBlock(h,x-1,z+1), testForBlock(h-1,x,z+1), testForBlock(h-1,x-1,z+1)));
					vertices.push_back(Vec3(x+1,h,z+1));
					AO.push_back(calcAO(testForBlock(h,x+1,z+1), testForBlock(h-1,x,z+1), testForBlock(h-1,x+1,z+1)));
					vertices.push_back(Vec3(x+1,h+1,z+1));
					AO.push_back(calcAO(testForBlock(h,x+1,z+1), testForBlock(h+1,x,z+1), testForBlock(h+1,x+1,z+1)));
					vertices.push_back(Vec3(x,h+1,z+1));
					AO.push_back(calcAO(testForBlock(h,x-1,z+1), testForBlock(h+1,x,z+1), testForBlock(h+1,x-1,z+1)));
					addIndices(indices, index);
					addNormals(normals, Vec3(0,0,1));
				}
				if(z-1 < 0 || !blocks[h][x][z-1]){
					vertices.push_back(Vec3(x+1,h,z));
					AO.push_back(calcAO(testForBlock(h,x+1,z-1), testForBlock(h-1,x,z-1), testForBlock(h-1,x+1,z-1)));
					vertices.push_back(Vec3(x,h,z));
					AO.push_back(calcAO(testForBlock(h,x-1,z-1), testForBlock(h-1,x,z-1), testForBlock(h-1,x-1,z-1)));
					vertices.push_back(Vec3(x,h+1,z));
					AO.push_back(calcAO(testForBlock(h,x-1,z-1), testForBlock(h+1,x,z-1), testForBlock(h+1,x-1,z-1)));
					vertices.push_back(Vec3(x+1,h+1,z));
				 	AO.push_back(calcAO(testForBlock(h,x+1,z-1), testForBlock(h+1,x,z-1), testForBlock(h+1,x+1,z-1)));
					addIndices(indices, index);
					addNormals(normals, Vec3(0,0,-1));
				}
			}
		}
	}
	int numVertices = vertices.size()*3;
	float* vertexData = new float[numVertices];
	for(int i = 0; i < vertices.size(); ++i){
		vertexData[i*3+0] = vertices[i].x;
		vertexData[i*3+1] = vertices[i].y;
		vertexData[i*3+2] = vertices[i].z;
	}
	numIndices = indices.size();
	
	glGenVertexArrays(1, &vaoID);
	glBindVertexArray(vaoID);

	glGenBuffers(1, &iboID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iboID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int)*indices.size(), &indices[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	glGenBuffers(1, &vboID);
	glBindBuffer(GL_ARRAY_BUFFER, vboID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float)*numVertices, vertexData, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, false, 0, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenBuffers(1, &vboaoID);
	glBindBuffer(GL_ARRAY_BUFFER, vboaoID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float)*AO.size(), &AO[0], GL_STATIC_DRAW);
	glVertexAttribPointer(3, 1, GL_FLOAT, false, 0, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenBuffers(1, &vbonID);
	glBindBuffer(GL_ARRAY_BUFFER, vbonID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float)*normals.size()*3, &normals[0].x, GL_STATIC_DRAW);
	glVertexAttribPointer(2, 3, GL_FLOAT, false, 0, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);
}

void VoxelTerrain::addIndices(std::vector<int> &indices, int &index){
	indices.push_back(index);
	indices.push_back(++index);
	indices.push_back(++index);
	indices.push_back(index);
	indices.push_back(++index);
	indices.push_back(index-3);
	++index;
}

void VoxelTerrain::addNormals(std::vector<Vec3> &normals, Vec3 normal){
	normals.push_back(normal);
	normals.push_back(normal);
	normals.push_back(normal);
	normals.push_back(normal);
}

void VoxelTerrain::addAO(std::vector<float> &AO, float occlusion){
	AO.push_back(occlusion);
	AO.push_back(occlusion);
	AO.push_back(occlusion);
	AO.push_back(occlusion);
}

float VoxelTerrain::calcAO(int side1, int side2, int corner){
	if(side1 && side2)
		return 0;
	return 3 - (side1 + side2 + corner);
}

int VoxelTerrain::testForBlock(int h, int x, int z){
	if(h >= height || h < 0)
		return 0;
	if(x >= size || x < 0)
		return 0;
	if(z >= size || z < 0)
		return 0;
	return blocks[h][x][z];
}

void VoxelTerrain::checkCollisions(Vec3 oldPos, Vec3 &posDelta){
	Vec3 newPos = oldPos + posDelta;

	float dist = 0.16f;
	Vec3 player_points[8] = {Vec3(-dist,-dist,-dist), Vec3(dist,-dist,-dist), Vec3(dist,-dist,dist), Vec3(-dist,-dist,dist), Vec3(-dist,1.6f,-dist), Vec3(dist,1.6f,-dist), Vec3(dist,1.6f,dist), Vec3(-dist,1.6f,dist)};

	for(int i = 0; i < 8; ++i){
		//Making sure we are not solving collisions more than once
		newPos = oldPos + posDelta;
		
		Vec3 previousBlock = Vec3(fastFloor(player_points[i].x+oldPos.x), fastFloor(player_points[i].y+oldPos.y), fastFloor(player_points[i].z+oldPos.z));
		Vec3 newBlock = Vec3(fastFloor(player_points[i].x+newPos.x), fastFloor(player_points[i].y+newPos.y), fastFloor(player_points[i].z+newPos.z));
		for(int j = 0; j < 3; ++j){
			if(previousBlock[j] == newBlock[j])
				continue;
			if(!blocks[(int)newBlock.y][(int)newBlock.x][(int)newBlock.z])
				continue;
			//Collision detected
			Vec3 normal = Vec3(0,0,0);
			normal[j] = -(newBlock[j] - previousBlock[j]);
			Vec3 tangent;
			if(normal.equals(Vec3(0,1,0)))
				tangent = cross(Vec3(1,0,0), normal).normalize();
			else
				tangent = cross(Vec3(0,1,0), normal).normalize();
			Vec3 newPosDelta = tangent * dot(tangent, posDelta);
			posDelta[j] = newPosDelta[j];
		}
	}
}

void VoxelTerrain::input(){
	
}

void VoxelTerrain::update(){
	
}

void VoxelTerrain::render(){
	glBindVertexArray(vaoID);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(2);
	glEnableVertexAttribArray(3);
	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iboID);
	glDrawElements(GL_TRIANGLES, numIndices, GL_UNSIGNED_INT, 0);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(2);
	glDisableVertexAttribArray(3);
	
	glBindVertexArray(0);
}

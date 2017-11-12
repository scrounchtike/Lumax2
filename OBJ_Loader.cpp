
#include "OBJ_Loader.hpp"

#include "math.hpp"

std::vector<float> OBJ_Loader::verticesIndexed;
std::vector<float> OBJ_Loader::texCoordsIndexed;
std::vector<float> OBJ_Loader::normalsIndexed;

void RawModel::calcTangents(){
	std::vector<float> tangents;
	for(int i = 0; i < numVertices/3; ++i){
		Vec3 vertex1 = Vec3(vertices[i*9+0], vertices[i*9+1], vertices[i*9+2]);
		Vec3 vertex2 = Vec3(vertices[i*9+3], vertices[i*9+4], vertices[i*9+5]);
		Vec3 vertex3 = Vec3(vertices[i*9+6], vertices[i*9+7], vertices[i*9+8]);

		Vec2 uv1 = Vec2(texCoords[i*6+0], texCoords[i*6+1]);
		Vec2 uv2 = Vec2(texCoords[i*6+2], texCoords[i*6+3]);
		Vec2 uv3 = Vec2(texCoords[i*6+4], texCoords[i*6+5]);

		Vec3 edge1 = vertex3 - vertex1;
		Vec3 edge2 = vertex2 - vertex1;
		Vec2 deltaUV1 = uv3 - uv1;
		Vec2 deltaUV2 = uv2 - uv1;

		Vec3 tangent = (edge1*deltaUV2.y - edge2*deltaUV1.y)/(deltaUV1.x*deltaUV2.y - deltaUV1.y*deltaUV2.x);
		tangents.push_back(tangent.x);
		tangents.push_back(tangent.y);
		tangents.push_back(tangent.z);
	}
	this->tangents = new float[tangents.size()];
	for(int i = 0; i < tangents.size(); ++i)
		this->tangents[i] = tangents[i];

	hasTangents = true;
}

RawModel OBJ_Loader::loadModel(const std::string& fileName){
	verticesIndexed.resize(0);
	texCoordsIndexed.resize(0);
	normalsIndexed.resize(0);
	
	std::ifstream input;
	input.open(fileName);

	RawModel model;

	if(!input.good()){
		std::cerr << "Error while opening mesh file " << fileName << std::endl;
		return model;
	}

	std::vector<float> vertices;
	std::vector<float> texCoords;
	std::vector<float> normals;
	std::vector<int> indices;

	std::string line;
	while(std::getline(input, line)){
		if(!line.substr(0,2).compare("v ")){
			std::vector<std::string> data = OBJ_Loader::split(line, ' ');
			verticesIndexed.push_back(std::stof(data[1]));
			verticesIndexed.push_back(std::stof(data[2]));
			verticesIndexed.push_back(std::stof(data[3]));
		}else if(!line.substr(0,3).compare("vt ")){
			std::vector<std::string> data = OBJ_Loader::split(line, ' ');
			texCoordsIndexed.push_back(std::stof(data[1]));
			texCoordsIndexed.push_back(std::stof(data[2]));
		}else if(!line.substr(0,3).compare("vn ")){
			std::vector<std::string> data = OBJ_Loader::split(line, ' ');
			normalsIndexed.push_back(std::stof(data[1]));
			normalsIndexed.push_back(std::stof(data[2]));
			normalsIndexed.push_back(std::stof(data[3]));
		}else if(!line.substr(0,2).compare("f ")){
			//Assuming all data has been submitted
			std::vector<std::string> data = OBJ_Loader::split(line, ' ');
			for(int i = 0; i < 3; ++i){
				std::vector<std::string> vertex = OBJ_Loader::split(data[i+1], '/');
				int index = std::stoi(vertex[0])-1;
				int texIndex = std::stoi(vertex[1])-1;
				int normalIndex = std::stoi(vertex[2])-1;
				processRawVertex(index, texIndex, normalIndex, vertices, texCoords, normals);
			}
		}
	}

	std::cout << "Model statistics" << std::endl;
	std::cout << "vertices = " << vertices.size() << std::endl;
	std::cout << "indices = " << indices.size() << std::endl;
	std::cout << "texCoords = " << texCoords.size() << std::endl;
	std::cout << "normals = " << normals.size() << std::endl;

	model.vertices = new float[vertices.size()];
	for(int i = 0; i < vertices.size(); ++i)
		model.vertices[i] = vertices[i];
	model.texCoords = new float[texCoords.size()];
	for(int i = 0; i < texCoords.size(); ++i)
		model.texCoords[i] = texCoords[i];
	model.normals = new float[normals.size()];
	for(int i = 0; i < normals.size(); ++i)
		model.normals[i] = normals[i];

	model.numVertices = vertices.size()/3;
	model.numIndices = indices.size();

	return model;
}

//No indices for now since the loading does not support it.

// TODO: This needs to be implemented in the Model3D class and not in the loader
// This is irrelevant to the loader and simply adds constraints to it
Model3D* OBJ_Loader::getModel(RawModel model){
	if(model.hasTangents)
		return new Model3D(model.vertices, model.numVertices, model.texCoords, model.normals, model.tangents);
	else
		return new Model3D(model.vertices, model.numVertices, model.texCoords, model.normals);
}

void OBJ_Loader::processRawVertex(int index, int texCoordIndex, int normalIndex, std::vector<float> &vertices, std::vector<float> &texCoords, std::vector<float> &normals){
	vertices.push_back(verticesIndexed[index*3+0]);
	vertices.push_back(verticesIndexed[index*3+1]);
	vertices.push_back(verticesIndexed[index*3+2]);

	texCoords.push_back(texCoordsIndexed[texCoordIndex*2+0]);
	texCoords.push_back(texCoordsIndexed[texCoordIndex*2+1]);

	normals.push_back(normalsIndexed[normalIndex*3+0]);
	normals.push_back(normalsIndexed[normalIndex*3+1]);
	normals.push_back(normalsIndexed[normalIndex*3+2]);
}

void OBJ_Loader::processVertex(int index, int texCoordIndex, int normalIndex, std::vector<float> &vertices, std::vector<int> &indices, std::vector<float> &texCoords,std::vector<float> &normals){
	//1. is the index new?
	
}
	
template<typename Out>
void OBJ_Loader::split(const std::string &s, char delim, Out result){
    std::stringstream ss;
    ss.str(s);
    std::string item;
    while (std::getline(ss, item, delim)) {
        *(result++) = item;
    }
}

std::vector<std::string> OBJ_Loader::split(const std::string &s, char delim){
	std::vector<std::string> elems;
	split(s, delim, std::back_inserter(elems));
	return elems;
}

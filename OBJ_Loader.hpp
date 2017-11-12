#ifndef OBJ_LOADER_HPP
#define OBJ_LOADER_HPP

#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>

#include "RL/Model3D.hpp"

struct RawModel{
	float* vertices;
	int* indices;
	float* texCoords;
	float* normals;
	float* tangents;
	int numVertices;
	int numIndices;
	bool hasTangents = false;

	//TODO: Move calc tangents function in the Model3D class since this has nothing to do with loading
	void calcTangents();
};

class OBJ_Loader{
public:
	static RawModel loadModel(const std::string& fileName);
	static Model3D* getModel(RawModel model);

	template<typename Out>
	static void split(const std::string &s, char delim, Out result);
	static std::vector<std::string> split(const std::string &s, char delim);
private:
	static void processVertex(int index, int texCoordIndex, int normalIndex, std::vector<float>& vertices, std::vector<int>& indices, std::vector<float>& texCoords, std::vector<float>& normals);
	static void processRawVertex(int index, int texCoordIndex, int normalIndex, std::vector<float>& vertices, std::vector<float>& texCoords, std::vector<float>& normals);
	
	static std::vector<float> verticesIndexed;
	static std::vector<float> texCoordsIndexed;
	static std::vector<float> normalsIndexed;
};

#endif

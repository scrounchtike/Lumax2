
#include <vector>
#include <fstream>

#include "../math.hpp"

#include "TerrainDX11.hpp"

TerrainDX11::TerrainDX11() {

}

TerrainDX11::TerrainDX11(const TerrainDX11& other) { }
TerrainDX11::~TerrainDX11() { }

void TerrainDX11::input() {

}

void TerrainDX11::update() {

}

void TerrainDX11::render() {
	terrainModel->render();
}

bool TerrainDX11::initialize(RenderingContextDX11* dx11, const std::string& setupFile) {
	this->dx11 = dx11;

	bool result;

	result = loadSetupFile(setupFile);
	assert(result);
	if (!result) return false;

	//result = loadHeightMap();
	result = loadRAWHeightMap();
	assert(result);
	if (!result) return false;

	// Laod the X-Z coordinates of the terrain
	int i, j, index;
	for (j = 0; j < terrainHeight; ++j) {
		for (i = 0; i < terrainWidth; ++i) {
			index = j * terrainWidth + i;

			// Set the X-Z coordinates
			heightMap[index].x = (float)i;
			heightMap[index].z = -(float)j;

			// Move the terrain depth into the positive range
			heightMap[index].z += (float)(terrainHeight - 1);

			// Scale the height
			heightMap[index].y /= terrainScale;
		}
	}

	//result = buildTerrainModel();
	buildTerrain2();
	assert(result);
	if (!result) return false;

	cleanUpHeightMap();

	return result;
}

bool TerrainDX11::loadSetupFile(const std::string& setupFile) {
	terrainFilename = new char[256];

	std::ifstream fileInput;
	fileInput.open(setupFile.c_str());
	if (fileInput.fail())
		return false;

	char input;
	fileInput.get(input);
	while (input != ':')
		fileInput.get(input);

	fileInput >> terrainFilename;

	fileInput.get(input);
	while (input != ':')
		fileInput.get(input);

	fileInput >> terrainHeight;

	fileInput.get(input);
	while (input != ':')
		fileInput.get(input);

	fileInput >> terrainWidth;

	fileInput.get(input);
	while (input != ':')
		fileInput.get(input);

	fileInput >> terrainScale;

	fileInput.close();

	return true;
}

bool TerrainDX11::loadHeightMap() {
	heightMap = new HeightMap[terrainWidth * terrainHeight];
	if (!heightMap)
		return false;

	FILE* file = fopen(terrainFilename, "rb");
	unsigned char info[54];
	fread(info, sizeof(unsigned char), 54, file);

	int width = *(int*)&info[18];
	int height = *(int*)&info[22];

	int size = 3 * width * height;
	unsigned char* data = new unsigned char[size];
	fread(data, sizeof(unsigned char), size, file);
	fclose(file);

	int i, j, k;
	int index;
	k = 0;
	for (j = 0; j < terrainHeight; ++j) {
		for (i = 0; i < terrainWidth; ++i) {
			// Bitmaps are upside down, so load bottom first
			//index = i + terrainWidth * (terrainHeight - 1 - j);
			index = j * terrainWidth + i;

			// Get the gray scale value from the bitmap
			float heightTerrain = data[k];

			// Store the pixel as the height in the height map array
			heightMap[index].y = (float)heightTerrain;

			k += 3;
		}
		++k;
	}

	// Release the bitmap image data
	delete[] data;
	data = 0;

	// Release the terrain filename
	delete[] terrainFilename;
	terrainFilename = 0;

	return true;
}

bool TerrainDX11::loadRAWHeightMap() {
	FILE* filePtr;
	int error, i, j, index;
	unsigned long long imageSize, count;
	unsigned short* rawImage;

	heightMap = new HeightMap[terrainWidth * terrainHeight];
	if (!heightMap)
		return false;

	error = fopen_s(&filePtr, terrainFilename, "rb");
	if (error != 0) {
		OutputDebugString("Error: Failed to open RAW heightmap during terrain intialization\n");
		assert(false);
		return false;
	}

	// Allocate storage for RAW data
	imageSize = terrainHeight * terrainHeight;
	rawImage = new unsigned short[imageSize];
	if (!rawImage) {
		assert(false);
		return false;
	}

	// Read the RAW data
	count = fread(rawImage, sizeof(unsigned short), imageSize, filePtr);
	if (count != imageSize) {
		OutputDebugString("Error: Failed to read RAW file data. Count did not match expected size\n");
		assert(false);
		return false;
	}

	fclose(filePtr);

	for (j = 0; j < terrainHeight; ++j) {
		for (i = 0; i < terrainWidth; ++i) {
			index = j * terrainWidth + i;
			heightMap[index].y = (float)rawImage[index];
		}
	}

	delete[] rawImage;
	rawImage = 0;

	delete[] terrainFilename;
	terrainFilename = 0;

	return true;
}

void TerrainDX11::buildTerrain2() {
	int faceCount = (terrainWidth - 1) * (terrainHeight - 1);
	int vertexCount = terrainWidth * terrainHeight;
	int indexCount = faceCount * 6;

	Vec3* vertices = new Vec3[vertexCount];
	Vec2* texCoords = new Vec2[vertexCount];
	Vec3* normals = new Vec3[vertexCount];

	Vec3* tangents = new Vec3[vertexCount];
	Vec3* bitangents = new Vec3[vertexCount];

	int* indices = new int[indexCount];

	Vec3* faceNormals = new Vec3[faceCount * 2];

	// Calculate indices + face normals
	int index = 0;
	for (int j = 0; j < (terrainHeight - 1); ++j) {
		for (int i = 0; i < (terrainWidth - 1); ++i) {
			int index0 = j * terrainWidth + i;
			int index1 = j * terrainWidth + (i + 1);
			int index2 = (j + 1) * terrainWidth + (i + 1);
			int index3 = (j + 1) * terrainWidth + i;

			// Indices
			indices[index++] = index0;
			indices[index++] = index1;
			indices[index++] = index2;
			indices[index++] = index2;
			indices[index++] = index3;
			indices[index++] = index0;

			// Face normals
			Vec3 vertex0 = Vec3(heightMap[index0].x, heightMap[index0].y, heightMap[index0].z);
			Vec3 vertex1 = Vec3(heightMap[index1].x, heightMap[index1].y, heightMap[index1].z);
			Vec3 vertex2 = Vec3(heightMap[index2].x, heightMap[index2].y, heightMap[index2].z);
			Vec3 vertex3 = Vec3(heightMap[index3].x, heightMap[index3].y, heightMap[index3].z);

			Vec3 v0 = vertex3 - vertex1;
			Vec3 v1 = vertex0 - vertex1;
			Vec3 v2 = vertex3 - vertex1;
			Vec3 v3 = vertex2 - vertex1;

			Vec3 normal0 = cross(v0, v1);
			Vec3 normal1 = cross(v3, v2);

			int faceNormalIndex = (j * (terrainWidth-1)) + i;
			faceNormals[faceNormalIndex * 2 + 0] = normal0;
			faceNormals[faceNormalIndex * 2 + 1] = normal1;
			
		}
	}

	for (int j = 0; j < terrainHeight; ++j) {
		for (int i = 0; i < terrainWidth; ++i) {
			int index = j * terrainWidth + i;
			vertices[index] = Vec3(heightMap[index].x, heightMap[index].y, heightMap[index].z);
			texCoords[index] = Vec2((float)i / (float)terrainWidth, (float)j / (float)terrainHeight);

			//Calculate the vertex normals
			Vec3 normal;

			if (((i - 1) >= 0) && ((j - 1) >= 0)){
				index = ((j - 1) * (terrainWidth - 1)) + (i - 1);
				normal += faceNormals[index * 2 + 1];
			}

			// Bottom right face.
			if ((i<(terrainWidth - 1)) && ((j - 1) >= 0)){
				index = ((j - 1) * (terrainWidth - 1)) + i;
				normal += faceNormals[index * 2 + 0];
				normal += faceNormals[index * 2 + 1];
			}

			// Upper left face.
			if (((i - 1) >= 0) && (j<(terrainHeight - 1))){
				index = (j * (terrainWidth - 1)) + (i - 1);
				normal += faceNormals[index * 2 + 0];
				normal += faceNormals[index * 2 + 1];
			}

			// Upper right face.
			if ((i < (terrainWidth - 1)) && (j < (terrainHeight - 1))){
				index = (j * (terrainWidth - 1)) + i;
				normal += faceNormals[index * 2 + 0];
			}

			index = j * terrainWidth + i;
			normal = normal.normalize();
			normals[index] = normal;

			Vec3 tangent, bitangent;
			Vec3 otherT, otherS;

			int indexOtherT = 0;
			if ((j + 1) >= terrainHeight)
				indexOtherT = (j - 1) * terrainWidth + i;
			else
				indexOtherT = (j + 1) * terrainWidth + i;
			otherT = Vec3(heightMap[indexOtherT].x, heightMap[indexOtherT].y, heightMap[indexOtherT].z);
			bitangent = vertices[index] - otherT;

			int indexOtherS = 0;
			if ((i + 1) >= terrainWidth)
				indexOtherS = j * terrainWidth + (i - 1);
			else
				indexOtherS = j * terrainWidth + (i + 1);
			otherS = Vec3(heightMap[indexOtherS].x, heightMap[indexOtherS].y, heightMap[indexOtherS].z);
			tangent = vertices[index] - otherS;

			tangent = tangent.normalize();
			bitangent = bitangent.normalize();

			tangents[index] = tangent;
			bitangents[index] = cross(normal, tangent);
		}
	}

	terrainModel = new Model3DDX11(dx11);
	terrainModel->initialize((float*)vertices, vertexCount, indices, indexCount, (float*)texCoords, (float*)normals, (float*)tangents, (float*)bitangents);

	delete[] vertices;
	vertices = 0;
	delete[] texCoords;
	texCoords = 0;
	delete[] normals;
	normals = 0;
	delete[] faceNormals;
	faceNormals = 0;
	delete[] tangents;
	tangents = 0;
	delete[] bitangents;
	bitangents = 0;
	delete[] indices;
	indices = 0;
}

bool TerrainDX11::buildTerrainModel() {
	int i, j, index, index1, index2, index3, index4;
	int indexTex;
	int normalIndex;

	vertexCount = (terrainHeight - 1) * (terrainWidth - 1) * 6;

	float* vertices = new float[vertexCount * 3];
	float* texCoords = new float[vertexCount * 2];
	float* normals = new float[vertexCount * 3];
	float* tangents = new float[vertexCount * 3];
	float* bitangents = new float[vertexCount * 3];

	// Load the model as triangles
	// TODO: Add support for indices (highly benificial for a terrain grid)
	index = 0;
	indexTex = 0;
	normalIndex = 0;
	for (j = 0; j < terrainHeight-1; ++j) {
		for (i = 0; i < terrainWidth-1; ++i) {
			// Get the indices of the 4 points of the quad
			index1 = (terrainWidth * j) + i;
			index2 = (terrainWidth * j) + (i + 1);
			index3 = (terrainWidth * (j + 1)) + i;
			index4 = (terrainWidth * (j + 1)) + (i + 1);

			// Triangle 1 - Upper Left
			vertices[index++] = heightMap[index1].x;
			vertices[index++] = heightMap[index1].y;
			vertices[index++] = heightMap[index1].z;
			texCoords[indexTex++] = 0;
			texCoords[indexTex++] = 0;
			//normals[normalIndex++] = 0.3f;
			//normals[normalIndex++] = 1;
			//normals[normalIndex++] = 0;

			// Triangle 1 - Upper Right
			vertices[index++] = heightMap[index2].x;
			vertices[index++] = heightMap[index2].y;
			vertices[index++] = heightMap[index2].z;
			texCoords[indexTex++] = 1;
			texCoords[indexTex++] = 0;
			//normals[normalIndex++] = 0.3f;
			//normals[normalIndex++] = 1;
			//normals[normalIndex++] = 0;

			// Triangle 1 - Bottom Left
			vertices[index++] = heightMap[index3].x;
			vertices[index++] = heightMap[index3].y;
			vertices[index++] = heightMap[index3].z;
			texCoords[indexTex++] = 0;
			texCoords[indexTex++] = 1;
			//normals[normalIndex++] = 0.3f;
			//normals[normalIndex++] = 1;
			//normals[normalIndex++] = 0;

			// Triangle 2 - Bottom Left
			vertices[index++] = heightMap[index3].x;
			vertices[index++] = heightMap[index3].y;
			vertices[index++] = heightMap[index3].z; 
			texCoords[indexTex++] = 0;
			texCoords[indexTex++] = 1;
			//normals[normalIndex++] = 0.3f;
			//normals[normalIndex++] = 1;
			//normals[normalIndex++] = 0;

			// Triangle 2 - Upper Right
			// Triangle 2 - Upper Right
			vertices[index++] = heightMap[index2].x;
			vertices[index++] = heightMap[index2].y;
			vertices[index++] = heightMap[index2].z;
			texCoords[indexTex++] = 1;
			texCoords[indexTex++] = 0;
			//normals[normalIndex++] = 0.3f;
			//normals[normalIndex++] = 1;
			//normals[normalIndex++] = 0;

			// Triangle 2 - Bottom Right
			vertices[index++] = heightMap[index4].x;
			vertices[index++] = heightMap[index4].y;
			vertices[index++] = heightMap[index4].z;
			texCoords[indexTex++] = 1;
			texCoords[indexTex++] = 1;
			//normals[normalIndex++] = 0.3f;
			//normals[normalIndex++] = 1;
			//normals[normalIndex++] = 0;
		}
	}

	
	float vertex1[3], vertex2[3], vertex3[3], vector1[3], vector2[3], sum[3], length;
	float* faceNormals = new float[(terrainWidth - 1) * (terrainHeight - 1) * 3];
	float* vertexNormals = new float[terrainWidth * terrainHeight * 3];

	int indexNormal = 0;

	for (j = 0; j<(terrainHeight - 1); j++){
		for (i = 0; i<(terrainWidth - 1); i++){
			index1 = ((j + 1) * terrainWidth) + i;      // Bottom left vertex.
			index2 = ((j + 1) * terrainWidth) + (i + 1);  // Bottom right vertex.
			index3 = (j * terrainWidth) + i;          // Upper left vertex.
														// Get three vertices from the face.
			vertex1[0] = heightMap[index1].x;
			vertex1[1] = heightMap[index1].y;
			vertex1[2] = heightMap[index1].z;

			vertex2[0] = heightMap[index2].x;
			vertex2[1] = heightMap[index2].y;
			vertex2[2] = heightMap[index2].z;

			vertex3[0] = heightMap[index3].x;
			vertex3[1] = heightMap[index3].y;
			vertex3[2] = heightMap[index3].z;

			// Calculate the two vectors for this face.
			vector1[0] = vertex1[0] - vertex3[0];
			vector1[1] = vertex1[1] - vertex3[1];
			vector1[2] = vertex1[2] - vertex3[2];
			vector2[0] = vertex3[0] - vertex2[0];
			vector2[1] = vertex3[1] - vertex2[1];
			vector2[2] = vertex3[2] - vertex2[2];

			index = (j * (terrainWidth - 1)) + i;

			// Calculate the cross product of those two vectors to get the un-normalized value for this face normal.
			Vec3 normal = Vec3((vector1[1] * vector2[2]) - (vector1[2] * vector2[1]), (vector1[2] * vector2[0]) - (vector1[0] * vector2[2]), (vector1[0] * vector2[1]) - (vector1[1] * vector2[0]));

			// Calculate the length.
			normal = normal.normalize();

			faceNormals[index*3+0] = normal.x;
			faceNormals[index*3+1] = normal.y;
			faceNormals[index*3+2] = normal.z;
		}
	}

	// Now go through all the vertices and take a sum of the face normals that touch this vertex.
	for (j = 0; j<terrainHeight; j++){
		for (i = 0; i<terrainWidth; i++){
			// Initialize the sum.
			sum[0] = 0.0f;
			sum[1] = 0.0f;
			sum[2] = 0.0f;

			// Bottom left face.
			if (((i - 1) >= 0) && ((j - 1) >= 0))
			{
				index = ((j - 1) * (terrainWidth - 1)) + (i - 1);

				sum[0] += faceNormals[index *3+0];
				sum[1] += faceNormals[index *3+1];
				sum[2] += faceNormals[index *3+2];
			}

			// Bottom right face.
			if ((i<(terrainWidth - 1)) && ((j - 1) >= 0))
			{
				index = ((j - 1) * (terrainWidth - 1)) + i;

				sum[0] += faceNormals[index *3+0];
				sum[1] += faceNormals[index *3+1];
				sum[2] += faceNormals[index *3+2];
			}

			// Upper left face.
			if (((i - 1) >= 0) && (j<(terrainHeight - 1)))
			{
				index = (j * (terrainWidth - 1)) + (i - 1);

				sum[0] += faceNormals[index * 3 + 0];
				sum[1] += faceNormals[index * 3 + 1];
				sum[2] += faceNormals[index * 3 + 2];
			}

			// Upper right face.
			if ((i < (terrainWidth - 1)) && (j < (terrainHeight - 1)))
			{
				index = (j * (terrainWidth - 1)) + i;

				sum[0] += faceNormals[index * 3 + 0];
				sum[1] += faceNormals[index * 3 + 1];
				sum[2] += faceNormals[index * 3 + 2];
			}

			// Calculate the length of this normal.
			length = (float)sqrt((sum[0] * sum[0]) + (sum[1] * sum[1]) + (sum[2] * sum[2]));

			// Get an index to the vertex location in the height map array.
			index = (j * terrainWidth) + i;

			// Normalize the final shared normal for this vertex and store it in the height map array.
			vertexNormals[indexNormal++] = sum[0] / length;
			vertexNormals[indexNormal++] = sum[1] / length;
			vertexNormals[indexNormal++] = sum[2] / length;
			//vertexNormals[indexNormal++] = 0.3f;
			//vertexNormals[indexNormal++] = 1;
			//vertexNormals[indexNormal++] = 0;
		}
	}

	normalIndex = 0;
	for (j = 0; j < terrainHeight - 1; ++j) {
		for (i = 0; i < terrainWidth - 1; ++i) {
			// Get the indices of the 4 points of the quad
			index1 = (terrainWidth * j) + i;
			index2 = (terrainWidth * j) + (i + 1);
			index3 = (terrainWidth * (j + 1)) + i;
			index4 = (terrainWidth * (j + 1)) + (i + 1);

			// Triangle 1 - Upper Left
			normals[normalIndex++] = vertexNormals[index1 * 3 + 0];
			normals[normalIndex++] = vertexNormals[index1 * 3 + 1];
			normals[normalIndex++] = vertexNormals[index1 * 3 + 2];

			// Triangle 1 - Upper Right
			normals[normalIndex++] = vertexNormals[index2 * 3 + 0];
			normals[normalIndex++] = vertexNormals[index2 * 3 + 1];
			normals[normalIndex++] = vertexNormals[index2 * 3 + 2];

			// Triangle 1 - Bottom Left
			normals[normalIndex++] = vertexNormals[index3 * 3 + 0];
			normals[normalIndex++] = vertexNormals[index3 * 3 + 1];
			normals[normalIndex++] = vertexNormals[index3 * 3 + 2];

			// Triangle 2 - Bottom Left
			normals[normalIndex++] = vertexNormals[index3 * 3 + 0];
			normals[normalIndex++] = vertexNormals[index3 * 3 + 1];
			normals[normalIndex++] = vertexNormals[index3 * 3 + 2];

			// Triangle 2 - Upper Right
			// Triangle 2 - Upper Right
			normals[normalIndex++] = vertexNormals[index2 * 3 + 0];
			normals[normalIndex++] = vertexNormals[index2 * 3 + 1];
			normals[normalIndex++] = vertexNormals[index2 * 3 + 2];

			// Triangle 2 - Bottom Right
			normals[normalIndex++] = vertexNormals[index4 * 3 + 0];
			normals[normalIndex++] = vertexNormals[index4 * 3 + 1];
			normals[normalIndex++] = vertexNormals[index4 * 3 + 2];
		}
	}

	index = 0;
	indexTex = 0;

	int indexTangent = 0;
	int indexBitangent = 0;
	for (j = 0; j < terrainHeight - 1; ++j) {
		for (i = 0; i < terrainWidth - 1; ++i) {
			Vec3 vertex1Pos = Vec3(vertices[index * 3 + 0], vertices[index * 3 + 1], vertices[index * 3 + 2]);
			Vec2 vertex1Tex = Vec2(texCoords[indexTex * 2 + 0], texCoords[indexTex * 2 + 1]);
			Vec3 vertex1Normal = Vec3(normals[index * 3 + 0], normals[index * 3 + 1], normals[index * 3 + 2]);
			index++;
			indexTex++;

			Vec3 vertex2Pos = Vec3(vertices[index * 3 + 0], vertices[index * 3 + 1], vertices[index * 3 + 2]);
			Vec2 vertex2Tex = Vec2(texCoords[indexTex * 2 + 0], texCoords[indexTex * 2 + 1]);
			Vec3 vertex2Normal = Vec3(normals[index * 3 + 0], normals[index * 3 + 1], normals[index * 3 + 2]);
			index++;
			indexTex++;

			Vec3 vertex3Pos = Vec3(vertices[index * 3 + 0], vertices[index * 3 + 1], vertices[index * 3 + 2]);
			Vec2 vertex3Tex = Vec2(texCoords[indexTex * 2 + 0], texCoords[indexTex * 2 + 1]);
			Vec3 vertex3Normal = Vec3(normals[index * 3 + 0], normals[index * 3 + 1], normals[index * 3 + 2]);
			index++;
			indexTex++;

			Vec3 vertexPositions[3] = { vertex1Pos, vertex2Pos, vertex3Pos };
			Vec2 vertexTextures[3] = { vertex1Tex, vertex2Tex, vertex3Tex };
			Vec3 vertexNormals[3] = { vertex1Normal, vertex2Normal, vertex3Normal };

			Vec3 tangent, bitangent;

			calculateTangentSpace(vertexPositions, vertexTextures, vertexNormals, tangent, bitangent);
		
			tangents[indexTangent++] = tangent.x;
			tangents[indexTangent++] = tangent.y;
			tangents[indexTangent++] = tangent.z;
			bitangents[indexBitangent++] = bitangent.x;
			bitangents[indexBitangent++] = bitangent.y;
			bitangents[indexBitangent++] = bitangent.z;

			tangents[indexTangent++] = tangent.x;
			tangents[indexTangent++] = tangent.y;
			tangents[indexTangent++] = tangent.z;
			bitangents[indexBitangent++] = bitangent.x;
			bitangents[indexBitangent++] = bitangent.y;
			bitangents[indexBitangent++] = bitangent.z;

			tangents[indexTangent++] = tangent.x;
			tangents[indexTangent++] = tangent.y;
			tangents[indexTangent++] = tangent.z;
			bitangents[indexBitangent++] = bitangent.x;
			bitangents[indexBitangent++] = bitangent.y;
			bitangents[indexBitangent++] = bitangent.z;

			tangents[indexTangent++] = tangent.x;
			tangents[indexTangent++] = tangent.y;
			tangents[indexTangent++] = tangent.z;
			bitangents[indexBitangent++] = bitangent.x;
			bitangents[indexBitangent++] = bitangent.y;
			bitangents[indexBitangent++] = bitangent.z;

			tangents[indexTangent++] = tangent.x;
			tangents[indexTangent++] = tangent.y;
			tangents[indexTangent++] = tangent.z;
			bitangents[indexBitangent++] = bitangent.x;
			bitangents[indexBitangent++] = bitangent.y;
			bitangents[indexBitangent++] = bitangent.z;

			tangents[indexTangent++] = tangent.x;
			tangents[indexTangent++] = tangent.y;
			tangents[indexTangent++] = tangent.z;
			bitangents[indexBitangent++] = bitangent.x;
			bitangents[indexBitangent++] = bitangent.y;
			bitangents[indexBitangent++] = bitangent.z;
		}
	}

	delete[] faceNormals;
	faceNormals = 0;
	delete[] vertexNormals;
	vertexNormals = 0;

	terrainModel = new Model3DDX11(dx11);
	terrainModel->initialize(vertices, vertexCount, texCoords, normals, tangents, bitangents);

	delete[] tangents;
	tangents = 0;
	delete[] bitangents;
	bitangents = 0;
	delete[] normals;
	normals = 0;
	delete[] texCoords;
	texCoords = 0;
	delete[] vertices;
	vertices = 0;

	return true;
}

void TerrainDX11::calculateTangentSpace(Vec3 position[3], Vec2 texCoord[3], Vec3 normal[3], Vec3& tangent, Vec3& bitangent) {
	Vec3 vector1, vector2;
	Vec2 tuVector, tvVector;
	float den, length;

	vector1 = position[1] - position[0];
	vector2 = position[2] - position[0];

	tuVector = texCoord[1] - texCoord[0];
	tvVector = texCoord[2] - texCoord[0];

	den = 1.0f / (tuVector.x * tvVector.y - tuVector.y - tvVector.x);

	tangent.x = (tvVector[1] * vector1[0] - tvVector[0] * vector2[0]) * den;
	tangent.y = (tvVector[1] * vector1[1] - tvVector[0] * vector2[1]) * den;
	tangent.z = (tvVector[1] * vector1[2] - tvVector[0] * vector2[2]) * den;

	bitangent.x = (tuVector[0] * vector2[0] - tuVector[1] * vector1[0]) * den;
	bitangent.y = (tuVector[0] * vector2[1] - tuVector[1] * vector1[1]) * den;
	bitangent.z = (tuVector[0] * vector2[2] - tuVector[1] * vector1[2]) * den;

	tangent = tangent.normalize();
	bitangent = bitangent.normalize();
}

bool TerrainDX11::cleanUp() {
	cleanUpBuffers();

	return true;
}

void TerrainDX11::cleanUpHeightMap() {
	if (heightMap) {
		delete[] heightMap;
		heightMap = 0;
	}
}

void TerrainDX11::cleanUpBuffers() {

}
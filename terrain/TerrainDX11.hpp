
#pragma once

#ifndef TERRAIN_DX11_HPP
#define TERRAIN_DX11_HPP

#include <string>
#include <vector>

#include "../RL/Model3DDX11.hpp"

#include "../RAL/UsingDX11.hpp"

class TerrainDX11 {
public:
	TerrainDX11();
	TerrainDX11(const TerrainDX11&);
	~TerrainDX11();

	void input();
	void update();
	void render();

	bool initialize(RenderingContextDX11* dx11, const std::string& setupFile);
	bool cleanUp();
private:
	Model3DDX11* terrainModel;
	RenderingContextDX11* dx11;

	// Terrain State
	bool calcTangents = false;
	bool calcNormals = false;

	//HeightMap
	struct HeightMap {
		float x, y, z;
		float nx, ny, nz;
	};
	HeightMap* heightMap;

	char* terrainFilename;
	unsigned int terrainHeight, terrainWidth;
	float terrainScale;

	unsigned int vertexCount;

	bool loadSetupFile(const std::string& setupFile);
	bool loadHeightMap();
	bool loadHeightMapPNG();
	bool loadRAWHeightMap();
	bool buildTerrainModel();

	void buildTerrain2();

	void calculateTangentSpace(Vec3 position[3], Vec2 texCoord[3], Vec3 normal[3], Vec3& tangent, Vec3& bitangent);

	void cleanUpHeightMap();
	void cleanUpBuffers();
};

#endif
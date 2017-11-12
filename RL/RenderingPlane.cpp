
#include "RenderingPlane.hpp"

Model3D* RenderingPlane::planeModelVertices;
Model3D* RenderingPlane::planeModelTexCoords;
Model3D* RenderingPlane::planeModelNormals;
Model3D* RenderingPlane::planeModelTangents;

void RenderingPlane::initialize(RenderingContextDX11* dx11) {
	float vertices[12] = { -1,0,-1,-1,0,1,1,0,1,1,0,-1 };
	float texCoords[8] = { 0,0,0,1,1,1,1,0 };
	float normals[12] = { 0,1,0,0,1,0,0,1,0,0,1,0 };
	float tangents[12] = { 1,0,0,1,0,0,1,0,0,1,0,0 };
	float bitangents[12] = { 0,0,1,0,0,1,0,0,1,0,0,1 };
	int indices[6] = { 0,1,2,2,3,0 };

	planeModelVertices = new Model3D(vertices, 4, indices, 6);
	planeModelTexCoords = new Model3D(vertices, 4, indices, 6, texCoords);
	planeModelNormals = new Model3D(vertices, 4, indices, 6, texCoords, normals);
	planeModelTangents = new Model3D(vertices, 4, indices, 6, texCoords, normals, tangents, bitangents);
}

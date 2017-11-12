
#include "RenderingPlane.hpp"

Model3DDX11* RenderingPlane::planeModelVertices;
Model3DDX11* RenderingPlane::planeModelTexCoords;
Model3DDX11* RenderingPlane::planeModelNormals;
Model3DDX11* RenderingPlane::planeModelTangents;

void RenderingPlane::initialize(RenderingContextDX11* dx11) {
	float vertices[12] = { -1,0,-1,-1,0,1,1,0,1,1,0,-1 };
	float texCoords[8] = { 0,0,0,1,1,1,1,0 };
	float normals[12] = { 0,1,0,0,1,0,0,1,0,0,1,0 };
	float tangents[12] = { 1,0,0,1,0,0,1,0,0,1,0,0 };
	float bitangents[12] = { 0,0,1,0,0,1,0,0,1,0,0,1 };
	int indices[6] = { 0,1,2,2,3,0 };

	planeModelVertices = new Model3DDX11(dx11);
	planeModelVertices->initialize(vertices, 4, indices, 6);

	planeModelTexCoords = new Model3DDX11(dx11);
	planeModelTexCoords->initialize(vertices, 4, indices, 6, texCoords);
	
	planeModelNormals = new Model3DDX11(dx11);
	planeModelNormals->initialize(vertices, 4, indices, 6, texCoords, normals);

	planeModelTangents = new Model3DDX11(dx11);
	planeModelTangents->initialize(vertices, 4, indices, 6, texCoords, normals, tangents, bitangents);
}

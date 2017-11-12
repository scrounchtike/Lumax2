
#pragma once

#ifndef RENDERING_PLANE_HPP
#define RENDERING_PLANE_HPP

#include "../rendering/Model3D_DX11.hpp"

#include "../RAL/RenderingContextDX11.hpp"

class RenderingPlane {
public:
	static void initialize(RenderingContextDX11* dx11);

	static Model3DDX11* getModelWithVertices() { return planeModelVertices; }
	static Model3DDX11* getModelWithTexCoords() { return planeModelTexCoords; }
	static Model3DDX11* getModelWithNormals() { return planeModelNormals; }
	static Model3DDX11* getModelWithTangents() { return planeModelTangents; }
private:
	static Model3DDX11* planeModelVertices;
	static Model3DDX11* planeModelTexCoords;
	static Model3DDX11* planeModelNormals;
	static Model3DDX11* planeModelTangents;
};

#endif
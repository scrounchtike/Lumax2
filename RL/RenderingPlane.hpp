
#pragma once

#ifndef RENDERING_PLANE_HPP
#define RENDERING_PLANE_HPP

#include "../RL/Model3D.hpp"

#include "../RAL/RenderingContextDX11.hpp"

class RenderingPlane {
public:
	static void initialize(RenderingContextDX11* dx11);

	static Model3D* getModelWithVertices() { return planeModelVertices; }
	static Model3D* getModelWithTexCoords() { return planeModelTexCoords; }
	static Model3D* getModelWithNormals() { return planeModelNormals; }
	static Model3D* getModelWithTangents() { return planeModelTangents; }
private:
	static Model3D* planeModelVertices;
	static Model3D* planeModelTexCoords;
	static Model3D* planeModelNormals;
	static Model3D* planeModelTangents;
};

#endif
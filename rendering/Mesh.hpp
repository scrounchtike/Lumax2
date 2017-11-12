
#ifndef MESH_HPP
#define MESH_HPP

#include "../RL/Model3D.hpp"
#include "Material.hpp"

class Mesh{
public:
	Mesh(Model3D* model, Material* material);

	void update();
	void render();
private:
	Model3D* model;
	Material* material;
};

#endif

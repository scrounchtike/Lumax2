
#include "Mesh.hpp"

Mesh::Mesh(Model3D* model, Material* material) : model(model), material(material){
	
}

void Mesh::update(){
	
}

void Mesh::render(){
	material->use();
	model->fullRender();
}

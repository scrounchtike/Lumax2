
#include "Material.hpp"

Material::Material(Texture* texture, Shader* shader) : texture(texture), shader(shader), isTextured(true), numTextures(1), isColored(false){
	
}

Material::Material(Vec3 color, Shader* shader) : color(color), shader(shader), isTextured(false), isColored(true){
	
}

Material::Material(Texture* texture, Vec3 color, Shader* shader) : texture(texture), color(color), shader(shader), isTextured(true), numTextures(1), isColored(true){
	
}

void Material::use() const{
	shader->bind();
	//Way to auto assign uniforms
	if(isTextured)
		setUniformsTexture();
	if(isColored)
		setUniformsColor();
}

void Material::setUniformsColor() const{
	shader->setUniform3f("color", color.x, color.y, color.z);
}

void Material::setUniformsTexture() const{
	if(numTextures > 1){
		//Deal with multiple textures
	}else
		texture->bind();
}


#ifndef MATERIAL_HPP
#define MATERIAL_HPP

#include "../math.hpp"

#include "Texture.hpp"
#include "Shader.hpp"

class Material{
public:
	Material(Texture* texture, Shader* shader);
	Material(Vec3 color, Shader* shader);
	Material(Texture* texture, Vec3 color, Shader* shader);

	void use() const;
private:
	void setUniformsColor() const;
	void setUniformsTexture() const;

	bool isTextured = false;
	bool isColored = false;
	int numTextures = 0;
	
	Shader* shader;
	Texture* texture;
	Vec3 color;
};

#endif

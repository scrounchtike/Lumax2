
#include "Texture.hpp"

Texture::Texture(GLuint id) : texture_ID(id){
	
}

Texture::Texture(const std::string& fileName){
	int width, height, numComponents;
	unsigned char* image = stbi_load(fileName.c_str(), &width, &height, &numComponents, STBI_rgb_alpha);

	glGenTextures(1, &texture_ID);
	glBindTexture(GL_TEXTURE_2D, texture_ID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	
	stbi_image_free(image);
}

void Texture::bind(){
	glBindTexture(GL_TEXTURE_2D, texture_ID);
}


#ifndef BAKED_MODEL_3D_HPP
#define BAKED_MODEL_3D_HPP

#include "Model3D.hpp"

class BakedModel3D : public Model3D{
public:
	BakedModel3D(float* vertices, int numVertices, float* texCoords, float* normals, float* bakedTexCoords);
	BakedModel3D(const std::vector<float>& vertices, const std::vector<float>& texCoords, const std::vector<float>& normals, const std::vector<float>& bakedTexCoords);

	void addBakedTexCoords(float* bakedTexCoords);
	void addBakedTexCoords(const std::vector<float>& bakedTexCoords);

	void bindForRender();
	void render();
	void unbindForRender();

	void fullRender();
private:
	GLuint vbobtID;
	
	void createVBOBT(float* bakedTexCoords, int numTexCoords, int index, int size);
	void createVBOBT(const std::vector<float>& bakedTexCoords, int index, int size);
};

#endif

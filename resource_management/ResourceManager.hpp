
#ifndef RESOURCE_MANAGEMENT_HPP
#define RESOURCE_MANAGEMENT_HPP

#include <string>
#include <vector>
#include <map>

#include <boost/filesystem.hpp>

#include "../rendering/Texture.hpp"
#include "../rendering/Shader.hpp"
#include "../RL/Model3D.hpp"
#include "../RL/Model2D.hpp"

class ResourceManager{
public:
	ResourceManager(const std::string& resFolder);
	bool loadResFolder();
	void loadDir(boost::filesystem::path filePath);
	void registerTexture(boost::filesystem::path filePath);

	void loadTexture(boost::filesystem::path filePath);
	
	static void printFilesInDir(const std::string& dirName);

	Texture* addTexture(const std::string& textureFileName);
	Texture* addTexture(const std::string& textureName, const std::string& textureFileName);
	Texture* addTexture(const std::string& textureName, Texture* texture);
	//Since shaders are not in the res folder, we request a full path here.
	Shader*  addShader(const std::string& shaderName, const std::string& filePath);
	Model3D* addModel(const std::string& modelFileName);
	Model3D* addModel(const std::string& modelName, const std::string& modelFileName);
	Model3D* addModel(const std::string& modelName, Model3D* model);

	Texture* getTexture(const std::string& textureName) const;
	Shader* getShader(const std::string& shaderName) const;
	Model3D* getModel3D(const std::string& modelName) const;
	Model2D* getModel2D(const std::string& modelName) const;
	
private:
	const std::string resFolder;
	const std::string texturesFolder;
	const std::string modelsFolder;
	const std::string fontsFolder;

	std::map<std::string, Texture*> textures;
	std::map<std::string, Shader*> shaders;
	std::map<std::string, Model3D*> models_3D;
	std::map<std::string, Model2D*> models_2D;
};

#endif

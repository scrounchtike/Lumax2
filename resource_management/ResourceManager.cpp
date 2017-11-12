
#include "ResourceManager.hpp"

#include <iostream>

#include "../OBJ_Loader.hpp"
#include "TextureLoader.hpp"

using namespace boost::filesystem;

ResourceManager::ResourceManager(const std::string& resFolder) : resFolder(resFolder), texturesFolder(resFolder + "/textures/"), modelsFolder(resFolder + "/models/"), fontsFolder(resFolder + "/fonts/"){
	//Thsi will load all textures in the res folder. Not necessarily a good idea...
	//bool loaded = loadResFolder();
	if(!exists(resFolder.c_str()) || !is_directory(resFolder.c_str()))
		std::cout << "Failed to load res folder: An error occured" << std::endl;
}

bool ResourceManager::loadResFolder(){
	if(!exists(resFolder.c_str()))
		return false;
	if(!is_directory(resFolder.c_str()))
		return false;
	loadDir(path(resFolder));
	return true;
}

void ResourceManager::loadDir(path filePath){
	directory_iterator it_end; //Default construction points to past the end
	for(directory_iterator it(filePath); it != it_end; ++it){
		if(is_directory(it->status())){
			//Directory
			loadDir(it->path());
		}else{
			//File
			std::string ext = ".png";
			if(it->path().extension() == ext)
				registerTexture(it->path());
		}
	}
}

void ResourceManager::registerTexture(path filePath){
	//Too heavy to load all textures at startup.
	//TODO: Way to specify textures to load one at a time.
	
	//Texture* texture = new Texture(filePath.string());
	//std::string fileName = basename(filePath);
	//textures.insert(std::pair<const std::string&, Texture*>(fileName, texture));
}

void ResourceManager::printFilesInDir(const std::string &dirName){
	std::cout << "CPP " << __cplusplus << std::endl;

	if(exists(dirName.c_str())){
		if(is_directory(dirName.c_str())){
			copy(directory_iterator(dirName.c_str()), directory_iterator(),
					 std::ostream_iterator<directory_entry>(std::cout, "\n"));
		}
	}
}

Texture* ResourceManager::addTexture(const std::string &textureFileName){
	//Texture* texture = new Texture(TextureLoader::loadTextureLIBPNG(texturesFolder + textureFileName));
	Texture* texture = new Texture(textureFileName);
	textures.insert(std::pair<std::string, Texture*>(textureFileName, texture));
	return texture;
}

Texture* ResourceManager::addTexture(const std::string &textureName, const std::string &textureFileName){
	Texture* texture = new Texture(TextureLoader::loadTextureLIBPNG(texturesFolder + textureFileName));
	textures.insert(std::pair<std::string, Texture*>(textureName, texture));
	return texture;
}

Texture* ResourceManager::addTexture(const std::string &textureName, Texture *texture){
	textures.insert(std::pair<std::string, Texture*>(textureName, texture));
	return texture;
}

Shader* ResourceManager::addShader(const std::string& shaderName, const std::string &filePath){
	Shader* shader = new Shader(filePath);
	shaders.insert(std::pair<std::string, Shader*>(shaderName, shader));
	return shader;
}

Model3D* ResourceManager::addModel(const std::string &modelFileName){
	Model3D* model = OBJ_Loader::getModel(OBJ_Loader::loadModel(modelsFolder + modelFileName));
	models_3D.insert(std::pair<std::string, Model3D*>(modelFileName, model));
	return model;
}

Model3D* ResourceManager::addModel(const std::string &modelName, const std::string &modelFileName){
	Model3D* model = OBJ_Loader::getModel(OBJ_Loader::loadModel(modelsFolder + modelFileName));
	models_3D.insert(std::pair<std::string, Model3D*>(modelName, model));
	return model;
}

Model3D* ResourceManager::addModel(const std::string &modelName, Model3D *model){
	models_3D.insert(std::pair<std::string, Model3D*>(modelName, model));
	return model;
}

Texture* ResourceManager::getTexture(const std::string& textureName) const{
	return textures.at(textureName);
}

Shader* ResourceManager::getShader(const std::string& shaderName) const{
  return shaders.at(shaderName);
}

Model3D* ResourceManager::getModel3D(const std::string& modelName) const{
	return models_3D.at(modelName);
}

Model2D* ResourceManager::getModel2D(const std::string& modelName) const{
	return models_2D.at(modelName);
}

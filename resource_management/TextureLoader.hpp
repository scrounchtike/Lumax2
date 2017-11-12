
#ifndef TEXTURE_LOADER_HPP
#define TEXTURE_LOADER_HPP

#include <iostream>
#include <vector>
#include <string>

#include <boost/filesystem.hpp>

#ifdef __unix
#include "OpenGL/gl3.h"
#elif (defined _WIN32 || defined _WIN64)
#include <GL/glew.h>
#endif

struct PNG_INFO{
	int width, height;
	int bit_depth;
	int color_type;
	unsigned int row_bytes;
	unsigned char** row_pointers;
	PNG_INFO(int width_, int height_, int bit_depth_, int color_type_, unsigned int row_bytes_){
		width = width_; height = height_; bit_depth = bit_depth_; color_type = color_type_; row_bytes = row_bytes_;
	}
};

class TextureLoader{
public:
	static void resizeTextures(const std::string& dirName);
	static void resizeTextureDir(boost::filesystem::path dirPath);
	static void resizeTexture(boost::filesystem::path filePath, int widthTarget, int heightTarget);

	static GLuint loadTextureLIBPNG(const std::string& filePath);
	static GLuint loadTextureSTB(const std::string& filePath);

	static PNG_INFO* loadPNG_LIBPNG(const std::string& filePath);

	//static void resizeTexture(const std::string& filePath, int width, int height);
	static void writeTexture(const std::string& filePath, unsigned char** data, int width, int height, int bit_depth, int color_type);
	static void writeTexture(const std::string& filePath, PNG_INFO* png);

	//Temporary
	static void createTestTexture();
private:
};

#endif


#include "TextureLoader.hpp"

#include <windows.h>
#include <stdlib.h>
#include <stdio.h>

#include "../rendering/stb_image.h"
#include <png.h>

#pragma comment(lib, "libpng.lib")

using namespace boost::filesystem;

void TextureLoader::resizeTextures(const std::string& dirName){
	if(!exists(dirName.c_str())){
		std::cerr << "Directory name given to Texture Loader does not exist in main project directory. Make sure the directory is accessible from the project folder." << std::endl;
		return;
	}
	if(!is_directory(dirName.c_str())){
		std::cerr << "Name given to Texture Loader is not a directory. If you wish to load a single file, use the resizeTexture() function." << std::endl;
		return;
	}
	resizeTextureDir(path(dirName));
}

void TextureLoader::resizeTextureDir(path dirPath){
	directory_iterator it_end;
	for(directory_iterator it(dirPath); it != it_end; ++it){
		if(is_directory(it->status()))
			resizeTextureDir(it->path());
		else
			if(it->path().extension() == ".png")
				resizeTexture(it->path(), 512, 512);
	}
}

void TextureLoader::resizeTexture(path filePath, int widthTarget, int heightTarget){
	PNG_INFO* png = loadPNG_LIBPNG(filePath.string());

	int offsetX = (widthTarget - png->width) / 2.0;
	int offsetY = (heightTarget - png->height) / 2.0;

	//NOTE: Can't we just memcpy the first texture in the middle of the second?
	
	unsigned char** row_pointers = new unsigned char*[heightTarget];
	for(int i = 0; i < heightTarget; ++i){
		//TODO: Make use of png->row_bytes to create 2D array.
		row_pointers[i] = new unsigned char[widthTarget * 4];
		for(int j = 0; j < widthTarget; ++j){
			if(i >= offsetY && i < (offsetY+png->height) && j >= offsetX && j < (offsetX+png->width)){
				row_pointers[i][j*4+0] = png->row_pointers[i-offsetY][(j-offsetX)*4+0];
				row_pointers[i][j*4+1] = png->row_pointers[i-offsetY][(j-offsetX)*4+1];
				row_pointers[i][j*4+2] = png->row_pointers[i-offsetY][(j-offsetX)*4+2];
				row_pointers[i][j*4+3] = png->row_pointers[i-offsetY][(j-offsetX)*4+3];
			}else{
				//NOTE: set whole 4 bytes to int 0?
				row_pointers[i][j*4+0] = (unsigned char)0;
				row_pointers[i][j*4+1] = (unsigned char)0;
				row_pointers[i][j*4+2] = (unsigned char)0;
				row_pointers[i][j*4+3] = (unsigned char)0;
			}
		}
	}
	unsigned char** old_pointers = png->row_pointers;
	int oldHeight = png->height;
	png->row_pointers = row_pointers;
	png->row_bytes = png->row_bytes / png->width * widthTarget;
	png->width = widthTarget;
	png->height = heightTarget;

	for(int i = 0; i < oldHeight; ++i)
		free(old_pointers[i]);
	free(old_pointers);

	//TODO: Free old memory!

	writeTexture("res/test/test512x512.png", png);
}

GLuint TextureLoader::loadTextureLIBPNG(const std::string &filePath){
	PNG_INFO* png = loadPNG_LIBPNG(filePath);

	GLubyte* outData = (unsigned char*) malloc(png->row_bytes * png->height);
	for (int i = 0; i < png->height; i++)
		memcpy(outData+(png->row_bytes * (png->height-1-i)), png->row_pointers[i], png->row_bytes);
	
	//Using defaults parameters
	GLuint textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, png->width, png->height, 0, GL_RGB, GL_UNSIGNED_BYTE, outData);

	//Free data
	for(int i = 0; i < png->height; ++i)
		free(png->row_pointers[i]);
	free(png->row_pointers);
	free(outData);
	
	return textureID;
}

PNG_INFO* TextureLoader::loadPNG_LIBPNG(const std::string& filePath){
	FILE* fp = fopen(filePath.c_str(), "rb");
	if(!fp){
		OutputDebugString("Error: PNG file could not be loaded\n");
		std::cerr << "Error: PNG file, " << filePath << ", could not be loaded" << std::endl;
		return NULL;
	}
	unsigned char header[8];
	fread(header, 1, 8, fp);
	//Checking if file is a PNG file
	if(png_sig_cmp(header, 0, 8)){
		OutputDebugString("Error: file is not a PNG\n");
		std::cerr << "Error: File " << filePath << " is not a PNG." << std::endl;
		fclose(fp);
		return NULL;
	}

	png_structp png_struct = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	png_infop png_info = png_create_info_struct(png_struct);

	if(setjmp(png_jmpbuf(png_struct))){
		OutputDebugString("Error: Unable to load file\n");
		std::cerr << "Error: unable to load file " << filePath << std::endl;
		std::cerr << "Couldn't initialize jmp location of png struct" << std::endl;
		png_destroy_read_struct(&png_struct, &png_info, NULL);
		fclose(fp);
		return NULL;
	}

	png_init_io(png_struct, fp);
	png_set_sig_bytes(png_struct, 8);

	png_read_png(png_struct, png_info, PNG_TRANSFORM_STRIP_16 | PNG_TRANSFORM_PACKING | PNG_TRANSFORM_EXPAND, NULL);
	png_uint_32 width, height;
	int bit_depth;
	int color_type;
	int interlace_type;
	png_get_IHDR(png_struct, png_info, &width, &height, &bit_depth, &color_type,
							 &interlace_type, NULL, NULL);
	
	unsigned int row_bytes = png_get_rowbytes(png_struct, png_info);
	
	png_bytepp row_pointers = png_get_rows(png_struct, png_info);
	PNG_INFO* png = new PNG_INFO((int)width, (int)height, bit_depth, color_type, row_bytes);
	png->row_pointers = row_pointers;
	
	//png_destroy_read_struct(&png_struct, &png_info, NULL);
	fclose(fp);
	
	return png;
}

GLuint TextureLoader::loadTextureSTB(const std::string& filePath){
	int width, height, numComponents;
	unsigned char* image = stbi_load(filePath.c_str(), &width, &height, &numComponents, STBI_rgb_alpha);

	GLuint textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	//Free data
	stbi_image_free(image);
	
	return textureID;
}

void TextureLoader::writeTexture(const std::string &filePath, unsigned char **data, int width, int height, int bit_depth, int color_type){
	FILE* fp = fopen(filePath.c_str(), "wb");
	if(!fp)
		std::cerr << "Error: could not open and write to file " << filePath << std::endl;

	png_structp png_struct = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	if(!png_struct)
		std::cerr << "Error: could not create png struct while writing to file " << filePath << std::endl;
	png_infop png_info = png_create_info_struct(png_struct);
	if(!png_info)
		std::cerr << "Error: could not create png info struct while writing to file " << filePath << std::endl;

	png_init_io(png_struct, fp);

	if(setjmp(png_jmpbuf(png_struct)))
		std::cerr << "Error while writing to file " << filePath << std::endl;
	
	png_set_IHDR(png_struct, png_info, width, height, bit_depth, color_type, PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);
	png_write_info(png_struct, png_info);

	if(setjmp(png_jmpbuf(png_struct)))
		std::cerr << "Error while writing to file " << filePath << std::endl;

	png_write_image(png_struct, data);

	if(setjmp(png_jmpbuf(png_struct)))
		std::cerr << "Error while writing to file " << filePath << std::endl;

	png_write_end(png_struct, NULL);
	
	fclose(fp);
}

void TextureLoader::writeTexture(const std::string &filePath, PNG_INFO* png){
	writeTexture(filePath, png->row_pointers, png->width, png->height, png->bit_depth, png->color_type);
}

void TextureLoader::createTestTexture(){
	PNG_INFO* png = loadPNG_LIBPNG("res/test/resized_PISGA0.png");

	/*
	unsigned char** row_pointers = new unsigned char*[512];
	for(int i = 0; i < 512; ++i){
		row_pointers[i] = new unsigned char[512*4];
		for(int j = 0; j < 512; ++j){
			row_pointers[i][j*4+0] = (unsigned char)255;
			row_pointers[i][j*4+1] = (unsigned char)0;
			row_pointers[i][j*4+2] = (unsigned char)0;
			row_pointers[i][j*4+3] = (unsigned char)255;
		}
	}
	png->row_pointers = row_pointers;
	*/

	writeTexture("res/test/test.png", png);

	resizeTexture(path("res/test/test.png"), 512, 512);
}

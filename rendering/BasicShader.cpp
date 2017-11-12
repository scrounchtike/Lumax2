
#include "BasicShader.hpp"

#include <iostream>

GLuint BasicShader::program;
GLuint BasicShader::vertexShader, BasicShader::fragmentShader;

void initBasicShader(){

	//TODO: Is a fragment shader necessary for basic rendering?

	//Create Shader Objects
	BasicShader::vertexShader = glCreateShader(GL_VERTEX_SHADER);
	BasicShader::fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

	//Load source code into shaders
	std::string sourceVertex = loadShaderFile("shaders/basicShader.vs");
	const char* stringSourceVertex[1] = {sourceVertex.c_str()};
	GLint lengthSourceVertex[1] = {(GLint)sourceVertex.length()};
	glShaderSource(BasicShader::vertexShader, 1, stringSourceVertex, lengthSourceVertex);

	std::string sourceFragment = loadShaderFile("shaders/basicShader.fs");
	const char* stringSourceFragment[1] = {sourceFragment.c_str()};
	GLint lengthSourceFragment[1] = {(GLint)sourceFragment.length()};
	glShaderSource(BasicShader::fragmentShader, 1, stringSourceFragment, lengthSourceFragment);

	//Compile both shaders
	glCompileShader(BasicShader::vertexShader);
	glCompileShader(BasicShader::fragmentShader);
	
	checkErrorShader(BasicShader::vertexShader, GL_COMPILE_STATUS);
	checkErrorShader(BasicShader::fragmentShader, GL_COMPILE_STATUS);

	//Create the shader program
	/*
	The order of either the compile or the attachment of shaders do not matter.
	Shaders can be attached and then compiled, or compiled and attached hereafter.
	 */
	BasicShader::program = glCreateProgram();
	glAttachShader(BasicShader::program, BasicShader::vertexShader);
	glAttachShader(BasicShader::program, BasicShader::fragmentShader);

	//Link the program
	//Linking must be done after compilation and attachment of all shaders to the program
	glLinkProgram(BasicShader::program);
	checkErrorProgram(BasicShader::program, GL_LINK_STATUS);

	glUseProgram(BasicShader::program);
	GLenum error = glGetError();
	if(error)
		std::cout << error << std::endl;
	
}

void checkErrorShader(GLuint shader, GLenum pname){
	int success;
	glGetShaderiv(shader, pname, &success);
	if(!success){
		char errorLog[512];
		int length;
		glGetShaderInfoLog(shader, 512, &length, errorLog);
		//Print error log
		for(int i = 0; i < length; ++i){
			std::cout << errorLog[i];
		}
		std::cout << std::endl;
	}
}

void checkErrorProgram(GLuint program, GLenum pname){
	int success;
	glGetProgramiv(program, pname, &success);
	if(!success){
		char errorLog[512];
		int length;
		glGetProgramInfoLog(program, 512, &length, errorLog);
		//Print error log
		for(int i = 0; i < length; ++i){
			std::cout << errorLog[i];
		}
		std::cout << std::endl;
	}
}

void bindBasicShader(){
	glUseProgram(BasicShader::program);
}

void cleanUpBasicShader(){
	//Detach the shaders from the program
	glDetachShader(BasicShader::program, BasicShader::vertexShader);
	glDetachShader(BasicShader::program, BasicShader::fragmentShader);
	//Delete the actual shaders
	glDeleteShader(BasicShader::vertexShader);
	glDeleteShader(BasicShader::fragmentShader);
	//TODO: Delete the program
	glDeleteProgram(BasicShader::program);
}

std::string loadShaderFile(const std::string& fileName){
	std::ifstream file;
	file.open(fileName);

	if(!file.good()){
		std::cout << "Error while opening shader source file; perhaps the file is not in the /shaders subfolder?" << std::endl;
		return NULL;
	}

	std::string result = "";
	std::string line;
	while(std::getline(file, line)){
	  result.append(line + "\n");
	}

	file.close();
	
	return result;
}

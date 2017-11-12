
#include "App3.hpp"

#include "Quad.hpp"
#include "../utils/ShaderUtils.hpp"

GLuint App3::program;
GLuint App3::vertexShader;
GLuint App3::fragmentShader;
GLint  App3::uniformTranslationLocation;
GLint  App3::uniformZLocation;

void App3::init(){
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

	std::string sourceStringVertex = loadShader("applications/app3/shader.vs");
	const char* sourceVertex[1] = {sourceStringVertex.c_str()};
	const int sourceVertexLength[1] = {(int)sourceStringVertex.length()};
	glShaderSource(vertexShader, 1, sourceVertex, sourceVertexLength);

	std::string sourceStringFragment = loadShader("applications/app3/shader.fs");
	const char* sourceFragment[1] = {sourceStringFragment.c_str()};
	const int sourceFragmentLength[1] = {(int)sourceStringFragment.length()};
	glShaderSource(fragmentShader, 1, sourceFragment, sourceFragmentLength);

	//Compilation
	glCompileShader(vertexShader);
	glCompileShader(fragmentShader);

	//Check if an error occured during compilation
	checkErrorShader(vertexShader, GL_COMPILE_STATUS);
	checkErrorShader(fragmentShader, GL_COMPILE_STATUS);

	//Program creation
	GLuint prevProgram = glCreateProgram();
	glAttachShader(prevProgram, vertexShader);
	glAttachShader(prevProgram, fragmentShader);

	//Make the binaries retrievable
	//Must be done before linking
	glProgramParameteri(prevProgram, GL_PROGRAM_BINARY_RETRIEVABLE_HINT, GL_TRUE);

	//Reorganize the indices for attributes
	//Must also be done before linking
	//Does not work since we the program prioritizes the locations we indicated in the shader
	//Here, (location = ?) takes precedence over what we do:
	//Those calls are useless basically.
	glBindAttribLocation(prevProgram, 1, "color");
	glBindAttribLocation(prevProgram, 0, "position");

	//Linking
	glLinkProgram(prevProgram);

	//Check if an error occured during linking
	checkErrorProgram(prevProgram, GL_LINK_STATUS);

	//TODO: Does the program have to be linked before getting the binaries?
	//According to the documentation, yes.

	//Does not work even though it is in the official 4.1 GL documentation
	//createProgramFromBinaries(prevProgram);
	program = prevProgram;

	//Print platform independent info about uniforms
	printInfoUniformsARCH();
	//Print information about program attributes
	printAttributes(program);

	//Uniforms
	//TODO: Make function to check that uniform name is valid
	//-1 == uniform name not valid.
	uniformTranslationLocation = addUniform("translation");
	uniformZLocation = addUniform("z");
	//Set the uniform to a default value
	//The shader program needs to be bound
	glUseProgram(program);
	glUniform2f(uniformTranslationLocation, 0.0f, 0.2f);
	glUniform1f(uniformZLocation, 0.0f);

	//Print uniforms
	printUniforms(program);

	//Init quad for rendering
	initAppQuad();
}

void App3::input(){
	
}

void App3::update(){
	
}

void App3::render(){
	glUseProgram(program);
	glVertexAttrib3f(1, 0, 1, 0);
	//Render a simple quad for testing
	renderAppQuad();
}

void App3::cleanUp(){
	glDetachShader(program, vertexShader);
	glDetachShader(program, fragmentShader);
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	glDeleteProgram(program);

	//cleanUp app quad
	cleanUpAppQuad();
}

GLint App3::addUniform(const GLchar *name){
	GLint location = glGetUniformLocation(program, name);
	if(location == -1)
		std::cerr << "Requested uniform name is invalid or does not exist" << std::endl;
	return location;
}

void App3::checkErrorShader(GLuint shader, GLenum pname){
	GLint success;
	glGetShaderiv(shader, pname, &success);
	if(!success){
		GLchar error[512];
		GLint length;
		glGetShaderInfoLog(shader, 512, &length, error);
		for(int i = 0; i < length; ++i){
			std::cout << error[i];
		}
		std::cout << std::endl;
	}
}

void App3::checkErrorProgram(GLuint program, GLenum pname){
	GLint success;
	glGetProgramiv(program, pname, &success);
	if(!success){
		char error[512];
		int length;
		glGetProgramInfoLog(program, 512, &length, error);
		for(int i = 0; i < length; ++i){
			std::cout << error[i];
		}
		std::cout << std::endl;
	}
}

std::string App3::loadShader(const std::string& fileName){
	std::ifstream file;
	file.open(fileName);

	if(!file.good()){
		std::cerr << "Error while loading shader for App3" << std::endl;
		return NULL;
	}

	std::string line;
	std::string result = "";
	while(std::getline(file, line)){
		result += (line + '\n');
	}

	file.close();

	return result;
}

/*
/  Function not working
*/
void App3::createProgramFromBinaries(GLuint prevProgram){
	//Check that the program is linked
	int status;
	glGetProgramiv(prevProgram, GL_LINK_STATUS, &status);
	std::cout << "link = " << status << std::endl;
	
	int result;
	glGetIntegerv(GL_NUM_PROGRAM_BINARY_FORMATS, &result);
	std::cout << "num program binary formats = " << result << std::endl;
	glGetIntegerv(GL_PROGRAM_BINARY_FORMATS, &result);
	std::cout << "formats = " << result << std::endl;
	
	//Get number of bytes from previoud program
	GLint binaryLength;
	glGetProgramiv(prevProgram, GL_PROGRAM_BINARY_LENGTH, &binaryLength);

	GLint error = glGetError();
	std::cout << "error = " << error << std::endl;
	std::cout << GL_INVALID_OPERATION << std::endl;

	//Get the binaries from previous program
	std::cout << "LENGTH = " << binaryLength << std::endl;
	GLchar binaries[2048];
	GLenum format;
	GLsizei length;
	glGetProgramBinary(prevProgram, 2048, &length, &format, binaries);
	std::cout << "LENGTH = " << length << std::endl;

	program = glCreateProgram();
	glProgramBinary(program, format, binaries, length);
	//This supposedly links the program
	//Check if linking was successful
	checkErrorProgram(program, GL_LINK_STATUS);
	
}

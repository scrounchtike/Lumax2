
#include "Shader.hpp"

Shader::Shader(const std::string& fileName){
	vertexShader = createShader(fileName + ".vs", GL_VERTEX_SHADER);
	fragmentShader = createShader(fileName + ".fs", GL_FRAGMENT_SHADER);

	glCompileShader(vertexShader);
	checkError(vertexShader, GL_COMPILE_STATUS, shaderType);
	glCompileShader(fragmentShader);
	checkError(fragmentShader, GL_COMPILE_STATUS, shaderType);
	
	program = glCreateProgram();
	glAttachShader(program, vertexShader);
	glAttachShader(program, fragmentShader);

	glLinkProgram(program);
	checkError(program, GL_LINK_STATUS, programType);
}

Shader::~Shader(){
	
}

void Shader::bind() const{
	glUseProgram(program);
}

void Shader::addUniform(const std::string &uniformName){
	GLint location = glGetUniformLocation(program, uniformName.c_str());
	uniforms.insert(std::pair<std::string, int>(uniformName, location));
}

void Shader::setUniform1i(const std::string &uniformName, int value){
	glUniform1i(uniforms[uniformName], value);
}

void Shader::setUniform1f(const std::string &uniformName, float x){
	glUniform1f(uniforms[uniformName], x);
}

void Shader::setUniform2f(const std::string &uniformName, float x, float y){
	glUniform2f(uniforms[uniformName], x, y);
}

void Shader::setUniform2f(const std::string &uniformName, Vec2 &value){
	glUniform2f(uniforms[uniformName], value.x, value.y);
}

void Shader::setUniform3f(const std::string &uniformName, float x, float y, float z){
	glUniform3f(uniforms[uniformName], x, y, z);
}

void Shader::setUniform3f(const std::string &uniformName, Vec3 &value){
	glUniform3f(uniforms[uniformName], value.x, value.y, value.z);
}

void Shader::setUniform4f(const std::string &uniformName, float x, float y, float z, float w){
	glUniform4f(uniforms[uniformName], x, y, z, w);
}

void Shader::setUniform4f(const std::string &uniformName, Vec4 &value){
	glUniform4f(uniforms[uniformName], value.x, value.y, value.z, value.w);
}

void Shader::setUniform4f(const std::string &uniformName, Quaternion &value){
	glUniform4f(uniforms[uniformName], value.x, value.y, value.z, value.w);
}

void Shader::setUniformMatrix(const std::string &uniformName, float *matrix){
	glUniformMatrix4fv(uniforms[uniformName], 1, GL_TRUE, matrix);
}

void Shader::setUniformMatrix(const std::string &uniformName, Mat4 &matrix){
	glUniformMatrix4fv(uniforms[uniformName], 1, GL_TRUE, matrix.getHeadPointer());
}

GLuint Shader::createShader(const std::string& fileName, GLenum type){
	GLuint shader = glCreateShader(type);

	std::string shaderSourceString = loadShader(fileName);
	const GLchar* shaderSource[1] = {shaderSourceString.c_str()};
	GLint shaderSourceLength[1] = {(GLint)shaderSourceString.length()};
	glShaderSource(shader, 1, shaderSource, shaderSourceLength);

	return shader;
}

std::string Shader::loadShader(const std::string &fileName){
	std::ifstream file;
	file.open(fileName);

	if(!file.good()){
		std::cerr << "error while trying to read shader source file: " << fileName << std::endl;
		return NULL;
	}

	std::string line;
	std::string result = "";
	while(std::getline(file, line)){
		result += line + '\n';
	}

	return result;
}

void Shader::checkError(GLuint object, GLenum pname, TYPE type){
	GLint success;
	if(type == shaderType)
		glGetShaderiv(object, pname, &success);
	else if(type == programType)
		glGetProgramiv(object, pname, &success);
	if(!success){
		std::cerr << "An error occured with shader generation" << std::endl;
		GLchar errorBuf[512];
		GLint length;
		if(type == shaderType)
			glGetShaderInfoLog(object, 512, &length, errorBuf);
		else if(type == programType)
			glGetProgramInfoLog(object, 512, &length, errorBuf);
		for(int i = 0; i < length; ++i){
			std::cerr << errorBuf[i];
		}
		std::cout << std::endl;
	}
}

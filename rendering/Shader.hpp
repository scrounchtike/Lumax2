
#ifndef SHADER_HPP
#define SHADER_HPP

#ifdef __unix
#include "OpenGL/gl3.h"
#elif (defined _WIN32 || defined _WIN64)
#include <GL/glew.h>
#endif

#include <string>
#include <iostream>
#include <fstream>
#include <map>

#include "../math.hpp"

class Shader{
  enum TYPE{
		shaderType,
		programType
	};
public:
	Shader(const std::string& fileName);
	~Shader();

	void bind() const;

	void addUniform(const std::string& uniformName);

	void setUniform1i(const std::string& uniformName, int value);
	void setUniform1f(const std::string& uniformName, float x);
	void setUniform2f(const std::string& uniformName, float x, float y);
	void setUniform2f(const std::string& uniformName, Vec2& value);
	void setUniform3f(const std::string& uniformName, float x, float y, float z);
	void setUniform3f(const std::string& uniformName, Vec3& value);
	void setUniform4f(const std::string& uniformName, float x, float y, float z, float w);
	void setUniform4f(const std::string& uniformName, Vec4& value);
	void setUniform4f(const std::string& uniformName, Quaternion& value);
	void setUniformMatrix(const std::string& uniformName, float* matrix);
	void setUniformMatrix(const std::string& uniformName, Mat4& matrix);

	static void checkError(GLuint shader, GLenum pname, TYPE type);

	static GLuint createShader(const std::string& fileName, GLenum type);
	static std::string loadShader(const std::string& fileName);

private:
	GLuint vertexShader;
	GLuint fragmentShader;
	GLuint program;

	std::map<std::string, int> uniforms;
};

#endif

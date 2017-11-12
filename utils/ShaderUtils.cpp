
#include "ShaderUtils.hpp"

#include <iostream>
#include <vector>

void printAttribute(GLuint program, GLuint index){
	GLchar name[64];
	GLint length;
	GLint size;
	GLenum type;
	glGetActiveAttrib(program, index, 64, &length, &size, &type, name);

	GLint location = glGetAttribLocation(program, name);

	std::cout << "Attribute " << index << " at location " << location << ": ";
	for(int i = 0 ; i < length; ++i){
		std::cout << name[i];
	}
	std::cout << " type: " << resolveType(type) << " size: " << size << std::endl;
}

void printAttributes(GLuint program){
	GLint numAttributes;
	glGetProgramiv(program, GL_ACTIVE_ATTRIBUTES, &numAttributes);
	std::cout << "Program has " << numAttributes << " active attributes" << std::endl;
	for(int i = 0; i < numAttributes; ++i){
		printAttribute(program, i);
	}
}

void printInfoUniformsARCH(){
	GLint maxVertexUniformComponents;
	glGetIntegerv(GL_MAX_VERTEX_UNIFORM_COMPONENTS, &maxVertexUniformComponents);
	GLint maxFragmentUniformComponents;
	glGetIntegerv(GL_MAX_FRAGMENT_UNIFORM_COMPONENTS, &maxFragmentUniformComponents);
	GLint maxGeometryUniformComponents;
	glGetIntegerv(GL_MAX_GEOMETRY_UNIFORM_COMPONENTS, &maxGeometryUniformComponents);

	std::cout << "Maximum components of vertex uniforms = " << maxVertexUniformComponents << std::endl;
	std::cout << "Maximum components of fragment uniforms = " << maxFragmentUniformComponents << std::endl;
	std::cout << "Maximum components of geometry uniforms = " << maxGeometryUniformComponents << std::endl;

	GLint maxCombinedVertexStorage;
	glGetIntegerv(GL_MAX_COMBINED_VERTEX_UNIFORM_COMPONENTS, &maxCombinedVertexStorage);
	GLint maxCombinedFragmentStorage;
	glGetIntegerv(GL_MAX_COMBINED_FRAGMENT_UNIFORM_COMPONENTS, &maxCombinedFragmentStorage);
	GLint maxCombinedGeometryStorage;
	glGetIntegerv(GL_MAX_COMBINED_GEOMETRY_UNIFORM_COMPONENTS, &maxCombinedGeometryStorage);

	std::cout << "Max uniform storage of vertex = " << maxCombinedVertexStorage << std::endl;
	std::cout << "Max uniform storage of fragment = " << maxCombinedFragmentStorage << std::endl;
	std::cout << "Max uniform storage of geometry = " << maxCombinedGeometryStorage << std::endl;
	
}

void printUniforms(GLuint program){
	GLint numUniforms = 0;
	glGetProgramiv(program, GL_ACTIVE_UNIFORMS, &numUniforms);
	std::cout << "Program has " << numUniforms << " uniforms active" << std::endl;
	
	//std::vector<GLchar*> uniformNamesVector(numUniforms);
	//std::vector<GLuint> uniformIndices(numUniforms);
	//GLchar* uniformNames[];
	//GLuint uniformIndices[];
	
	//glGetUniformIndices(program, numUniforms, uniformNames, uniformIndices);
	
	for(int i = 0; i < numUniforms; ++i){
		//std::cout << "Uniform " << i << " has index: " << uniformIndices[i] << " and name: ";
		GLchar name[64];
		GLint length;
		glGetActiveUniformName(program, i, 64, &length, name);
		for(int j = 0; j < length; ++j){
			std::cout << name[j];
		}
		std::cout << std::endl;
	}
}

std::string resolveType(GLenum type){
	switch(type){
	case GL_INT:
		return "INT";
	case GL_FLOAT:
		return "FLOAT";
	case GL_DOUBLE:
		return "DOUBLE";
	case GL_HALF_FLOAT:
		return "HALF";
	case GL_FLOAT_VEC2:
		return "VEC2F";
	case GL_FLOAT_VEC3:
		return "VEC3F";
	case GL_FLOAT_VEC4:
		return "VEC4F";
	case GL_DOUBLE_VEC2:
		return "VEC2D";
	case GL_DOUBLE_VEC3:
		return "VEC3D";
	case GL_DOUBLE_VEC4:
		return "VEC4D";
	case GL_FLOAT_MAT2:
		return "MAT2F";
	case GL_FLOAT_MAT3:
		return "MAT3F";
	case GL_FLOAT_MAT4:
		return "MAT4F";
	case GL_DOUBLE_MAT2:
		return "MAT2D";
	case GL_DOUBLE_MAT3:
		return "MAT3D";
	case GL_DOUBLE_MAT4:
		return "MAT4D";
	default:
		return "Unknown Type";
	}
}


#ifndef DEBUG_RENDERER_HPP
#define DEBUG_RENDERER_HPP

#include <vector>

#include "Shader.hpp"
#include "../math.hpp"
#include "Camera.hpp"

//TODO: Remove references to OpenGL headers if we dont need them
/*
#ifdef __unix
#include "OpenGL/gl3.h"
#elif (defined _WIN32 || defined _WIN64)
#include <GL/glew.h>
#endif
*/

class Point{
public:
	Vec3 position;
	Vec3 color;
	Point(){}
	Point(Vec3 position_value, Vec3 color_value){
		position = position_value; color = color_value;
	}
};

class Line{
public:
	Vec3 start;
	Vec3 end;
	Vec3 color;
	Line(Vec3 start, Vec3 end, Vec3 color){
		this->start = start; this->end = end; this->color = color;
	}
};

class Quad{
public:
	Vec3 position;
	Vec3 color;
	Vec3 scale;
	Quad(){};
	Quad(Vec3 position, Vec3 scale, Vec3 color){
		this->position = position; this->scale = scale; this->color = color;
	}
};

class DebugRenderer{
public:
	static void init();
	static void cleanUp();

	static void update();
	static void render(Camera* camera);

	static void addPoint(Vec3 position, Vec3 color){
		points.push_back(Point(position, color));
	}
	static void addPoint(Point point){
		points.push_back(point);
	}
	static void addLine(Vec3 start, Vec3 end, Vec3 color){
		lines.push_back(Line(start, end, color));
	}
	static void addLine(Line line){
		lines.push_back(line);
	}
	static void addQuad(Vec3 position, Vec3 scale, Vec3 color){
		quads.push_back(Quad(position, scale, color));
	}
	static void addQuad(Quad quad){
		quads.push_back(quad);
	}
private:
	static void passCameraUniforms(Shader* shader, Camera* camera);
	static void renderPoints();
	static void renderLines();
	static void renderQuads();
	
	static std::vector<Point> points;
	static std::vector<Line>  lines;
	static std::vector<Quad>  quads;

	//Points
	static Shader* shaderPoints;
	static GLuint  vaoID_points;
	static GLuint  vboID_points;
	static GLuint  iboID_points;

	//Lines
	static Shader* shaderLines;
	static GLuint  vaoID_lines;
	static GLuint  vboID_lines;

	//Quads
	static Shader* shaderQuads;
	static GLuint  vaoID_quads;
	static GLuint  vboID_quads;
};

#endif

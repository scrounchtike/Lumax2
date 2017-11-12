
#ifndef POLYGON_CLIP
#define POLYGON_CLIP

#include <iostream>

#include <vector>
#include <string>

#include "../math.hpp"

class PolygonClass{
public:
	std::vector<Vec4> vertices;
};

class Plane{
public:
	Vec4 normal;
	float D;
	Plane(){};
	Plane(Vec4 normal, float D){
		this->normal = normal; this->D = D;
	}
	bool vertexIsVisible(Vec4 vertex){
		return Vector4f::dot(vertex, normal);
	}
	Vec4 intersectLine(Vec4 S, Vec4 E){
		//std::cout << "INTERSECT LINE" << std::endl;
		//std::cout << "S = ";
		//print(S);
		//std::cout << "E = ";
		//print(E);
		//std::cout << "normal = ";
		//print(normal);
		
		Vec4 line = E - S;
		float d = Vector4f::dot(normal, line);
		//std::cout << "d = " << d << std::endl;
		float t;
		if(fabs(d) > 1e-10){
			//float dot = Vector4f::dot(normal, S);
			//t = (-1.0 - Vector4f::dot(normal, S)) / d;
			t = (Vector4f::dot(normal, S)) / d;
		}else
			t = 1.0;

		//std::cout << "t = " << t << std::endl;
		
	  return (S + line*t);
	}
	static void print(Vec4 v){
		std::cout << v.x << " " << v.y << " " << v.z << " " << v.w << std::endl;
	}
};

enum PLANES_ENUM{ FRONT, LEFT, RIGHT, TOP, BOTTOM };

class ClipPlane{
public:
	Vec4 first;
	Vec4 start;
	bool first_encountered;
	bool first_was_inside;
	bool start_was_inside;
	ClipPlane* pnext;
	Plane plane;

	bool isVisible(Vec4 v){
		return (Vector4f::dot(v, plane.normal) >= -1.0);
	}

	ClipPlane(){ first_encountered = false; };

	void clipPlane(Vec4 vertex, PolygonClass& out, std::string vertexName);
	void closeEdge(PolygonClass& out);
	void output(Vec4 vertex, PolygonClass& out, std::string vertexName);

	PLANES_ENUM plane_name;
	void printSpaces();
	void printPlaneName();
};

class PolygonClip{
public:
	PolygonClip();
	PolygonClass clipPolygon(PolygonClass& polygon);
private:
	ClipPlane clippers[4];
};

#endif

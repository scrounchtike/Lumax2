
#include "polygon_clip.hpp"

#include <iostream>

int intersects = 0;

PolygonClip::PolygonClip(){
	//Init plane clippers
	clippers[TOP]    = ClipPlane();
	clippers[RIGHT]  = ClipPlane();
	clippers[BOTTOM]   = ClipPlane();
	clippers[LEFT] = ClipPlane();

	//Add references to make linked list
	clippers[LEFT].pnext = &clippers[RIGHT];
	clippers[RIGHT].pnext = &clippers[TOP];
	clippers[TOP].pnext = &clippers[BOTTOM];
	clippers[BOTTOM].pnext = NULL;

	//Set the planes
	//Plane normals are inverted since normal points INSIDE the visible space
	clippers[TOP].plane = Plane(Vec4(0,-1,0,0), 1.0);
	clippers[RIGHT].plane = Plane(Vec4(-1,0,0,0), 1.0);
	clippers[BOTTOM].plane = Plane(Vec4(0,1,0,0), -1.0);
	clippers[LEFT].plane = Plane(Vec4(1,0,0,0), -1.0);

	//TEMPORARY
	clippers[TOP].plane_name = TOP;
	clippers[RIGHT].plane_name = RIGHT;
	clippers[BOTTOM].plane_name = BOTTOM;
	clippers[LEFT].plane_name = LEFT;
}

PolygonClass PolygonClip::clipPolygon(PolygonClass& polygon){
	//Empty polygon
	PolygonClass out;
	
	for(int i = 0; i < polygon.vertices.size(); ++i){
		std::string name = "P" + std::to_string(i);
		clippers[0].clipPlane(polygon.vertices[i], out, name);
	}
	clippers[0].closeEdge(out);

	return out;
}

void ClipPlane::clipPlane(Vec4 vertex, PolygonClass& out, std::string vertexName){
	printSpaces();
	std::cout << "Clip vertex " << vertexName << ",";
	printPlaneName();
	std::cout << std::endl;
	
	bool vertex_inside;
	//vertex_inside = plane.vertexIsVisible(vertex);
	vertex_inside = isVisible(vertex);
	printSpaces();
	std::cout << "visible = " << vertex_inside << std::endl;
	Vec4 isect;

	//if(plane_name == BOTTOM)
	//std::cout << "FIRST FLAG = " << first_encountered << std::endl;

	//Is is it first vertex seen?
	if(!first_encountered){
		//First Vertex seen
		first_encountered = true;
		
		printSpaces();
		std::cout << "first ";
		printPlaneName();
		std::cout << ": " << vertexName << std::endl;
		
		first = vertex;
		first_was_inside = vertex_inside;
	}else if(vertex_inside ^ start_was_inside){
		//Not the first vertex
		//Does edge intersect plane?
		
		isect = plane.intersectLine(start, vertex);

		printSpaces();
		std::cout << "Intersect edge SV ";
		std::cout << "New I" << std::to_string(intersects++) << " = " << isect.x << " " << isect.y << " " << isect.z << std::endl;
		std::string newVertex = "I" + std::to_string(intersects-1);
		
		output(isect, out, newVertex);
	}

	if(vertex_inside)
		output(vertex, out, vertexName);

	start = vertex;
	printSpaces();
	std::cout << "S ";
	printPlaneName();
	std::cout << " = " << vertexName << std::endl;
	
	start_was_inside = vertex_inside;
}

void ClipPlane::closeEdge(PolygonClass& out){
	printSpaces();
	std::cout << "close ";
	printPlaneName();
	std::cout << std::endl;
	
	Vec4 isect;
	if(first_encountered){
		if(start_was_inside ^ first_was_inside){
			isect = plane.intersectLine(start, first);
			
			output(isect, out, "P-1");
		}

		//More Planes?
		if(pnext != NULL){
			pnext->closeEdge(out);
		}
		first_encountered = false;
	}
}

void ClipPlane::output(Vec4 vertex, PolygonClass &out, std::string vertexName){
	if(pnext != NULL)
		pnext->clipPlane(vertex, out, vertexName);
	else{
		out.vertices.push_back(vertex);
		printSpaces();
		std::cout << "output " << vertexName << std::endl;
	}
}

void ClipPlane::printSpaces(){
	int numSpaces = 0;
	if(plane_name == LEFT)
		numSpaces = 1;
	else if(plane_name == RIGHT)
		numSpaces = 2;
	else if(plane_name == TOP)
		numSpaces = 2;
	else if(plane_name == BOTTOM)
		numSpaces = 3;
	for(int i = 0; i < numSpaces; ++i){
		std::cout << " ";
	}
}

void ClipPlane::printPlaneName(){
	if(plane_name == TOP)
		std::cout << "TOP";
	else if(plane_name == RIGHT)
		std::cout << "RIGHT";
	else if(plane_name == BOTTOM)
		std::cout << "BOTTOM";
	else if(plane_name == LEFT)
		std::cout << "LEFT";
	else if(plane_name == FRONT)
		std::cout << "FRONT";
}

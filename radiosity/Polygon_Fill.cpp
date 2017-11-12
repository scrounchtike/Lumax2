
#include "Polygon_Fill.hpp"

#include <algorithm>

PolygonFill::PolygonFill(){
	
}

void PolygonFill::fillPolygon(PolygonClass &out){
	ymin = dimension-1;
	ymax = 0;

	//Find y minima vertex
	float y_min = 100;
	int index;
	for(int i = 0; i < out.vertices.size(); ++i){
		Vec4 vertex = out.vertices[i];
		if(vertex.y < y_min){
			y_min = vertex.y;
			index = i;
		}
	}
	
	for(int i = 0; i < out.vertices.size(); ++i){
		Vec3 S = out.vertices[(i+index)%out.vertices.size()].xyz();
		Vec3 E = out.vertices[(i+index+1)%out.vertices.size()].xyz();
		//std::cout << "START = " << S.x << " " << S.y << " " << S.z << std::endl;
		//Ignore horizontal lines
		if(S.y == E.y)
			continue;
		//Swap S and E if S.y if higher than E.y
		if(S.y > E.y){
			Vec3 temp = E;
			E = S;
			S = temp;
		}
		std::cout << "BIGGEST " << S.y << " " << E.y << std::endl;
		if(S.y < ymin)
			ymin = S.y;
		if(E.y > ymax)
			ymax = E.y;
		std::cout << "MAX = " << ymax << std::endl;
		scanEdge(S, E);
	}
}

void PolygonFill::fillCanvas(int canvas[100][100]){

	//testing intersections
	for(int y = 0; y < 100; ++y){
		std::cout << y << " set = " << intersections[y][0].x << std::endl;
	}
	
	std::cout << "min and max = " << ymin << " " << ymax << std::endl;
	for(int y = ymin; y < ymax; ++y){
		ScanInfo int1 = intersections[y][0];
		//std::cout << "SCAN LINE " << y << std::endl;
		if(!int1.init)
			continue;
		ScanInfo int2 = intersections[y][1];
		int numPixels = fabs(int2.x - int1.x);
		std::cout << "y = " << y << " " << numPixels << " " << int1.x << std::endl;
		for(int x = 0; x < numPixels; ++x){
			canvas[y][x+(int)int1.x] = 1.0;
		}
	}
}

void PolygonFill::scanEdge(Vec3 S, Vec3 E){
	std::cout << "SCAN EDGE" << std::endl;
	std::cout << S.x << " " << S.y << " to " << E.x << " " << E.y << std::endl;
	float ix = S.x;
	float iz = S.z;
	
	float y_dist = E.y-S.y;
	float dx = (E.x-S.x)/y_dist;
	float dz = (E.z-S.z)/y_dist;

	for(int j = S.y; j < E.y; ++j){
		addScanIntersection(j, ix, iz);
		ix += dx;
		iz += dz;
	}
}

void PolygonFill::addScanIntersection(int y, float sx, float sz){
	std::cout << "Add scan intersection " << y << " sx = " << sx << std::endl;
	if(!intersections[y][0].init)
		intersections[y][0] = ScanInfo(sx, sz);
	else
		intersections[y][1] = ScanInfo(sx, sz);
}

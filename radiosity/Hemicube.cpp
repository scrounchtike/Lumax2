
#include "Hemicube.hpp"

#include "../rendering/DebugRenderer.hpp"

#include <cmath>

const int   Hemicube::arrayRes;
const int   Hemicube::arrayDim;
float Hemicube::top_array[arrayDim][arrayDim];

bool clipTriangleAxis(PolygonClass& triangle, int c, PolygonClass& out, bool halfAxis);
void clipTriangleComponent(PolygonClass triangle, int c, float compFactor, PolygonClass& out, bool wzero);
Vec4 lerp(Vec4 A, Vec4 B, float alpha);

Hemicube::Hemicube(){
	//Init form factors for top side
	float dx, dy, dz;
	dx = dy = dz = 2.0 / arrayRes;
	float da = 4.0 / (arrayRes * arrayRes);

	float x, y;
	x = dx / 2.0;
	for(int i = 0; i < arrayDim; ++i){
		y = dy / 2.0;
		for(int j = 0; j < arrayDim; ++j){
			float r = x*x + y*y + 1.0;
			top_array[i][j] = da / (PI * r * r);
			y += dy;
		}
		x += dx;
	}
	
	clippers[FRONT] = ClipPlane();
	clippers[LEFT] = ClipPlane();
	clippers[RIGHT] = ClipPlane();
	clippers[TOP] = ClipPlane();
	clippers[BOTTOM] = ClipPlane();

	clippers[FRONT].pnext = &clippers[LEFT];
	clippers[LEFT].pnext = &clippers[RIGHT];
	clippers[RIGHT].pnext = &clippers[TOP];
	clippers[TOP].pnext = &clippers[BOTTOM];
	clippers[BOTTOM].pnext = NULL;

	clippers[FRONT].plane = Plane(Vec4(0,0,1,0), -1.0);
	clippers[LEFT].plane = Plane(Vec4(1,0,0,0), -1.0);
	clippers[RIGHT].plane = Plane(Vec4(-1,0,0,1), 1.0);
	clippers[TOP].plane = Plane(Vec4(0,-1,0,1), 1.0);
	clippers[BOTTOM].plane = Plane(Vec4(0,1,0,0), -1.0);

	clippers[FRONT].plane_name = FRONT;
	clippers[LEFT].plane_name = LEFT;
	clippers[RIGHT].plane_name = RIGHT;
	clippers[TOP].plane_name = TOP;
	clippers[BOTTOM].plane_name = BOTTOM;

	Vec3 position = Vec3(0,0,3);
	position = -position;
	Mat4 translation = Mat4().initTranslation(position.x, position.y, position.z);
	Mat4 viewMatrix0 = Mat4().initCamera(Vec3(0,0,-1), Vec3(0,1,0), Vec3(1,0,0));
	viewMatrix0.mul(translation);
	viewMatrices[FRONT] = viewMatrix0;
	Mat4 viewMatrix1 = Mat4().initCamera(Vec3(-1,0,0), Vec3(0,1,0), Vec3(0,0,-1));
	viewMatrix1.mul(translation);
	viewMatrices[LEFT] = viewMatrix1;

	Mat4 vm = viewMatrices[LEFT];
	std::cout << "Print view matrix" << std::endl;
	std::cout << vm.get(0, 0) << " " << vm.get(0, 1) << " " << vm.get(0, 2) << " " << vm.get(0, 3) << std::endl;
	std::cout << vm.get(1, 0) << " " << vm.get(1, 1) << " " << vm.get(1, 2) << " " << vm.get(1, 3) << std::endl;
	std::cout << vm.get(2, 0) << " " << vm.get(2, 1) << " " << vm.get(2, 2) << " " << vm.get(2, 3) << std::endl;
	std::cout << vm.get(3, 0) << " " << vm.get(3, 1) << " " << vm.get(3, 2) << " " << vm.get(3, 3) << std::endl;
}

void Hemicube::transformPolygon(PolygonClass& polygon){
	int currentFace = FRONT;
	
	std::cout << "Transforming polygon to Hemicube patches" << std::endl;
	PolygonClass transfer;
	Vec3 translation = Vec3(0,0,3);
	//1. Transform polygon vertices into eye's coordinate system
	std::cout << polygon.vertices.size() << std::endl;
	for(int i = 0; i < polygon.vertices.size(); ++i){
		Vec4 vertex = polygon.vertices[i];
		vertex.w = 1.0;
		//Vec4 vertex_eye = Vec4(vertex.x-dot(translation,Vec3(0,0,1)),vertex.y-dot(translation,Vec3(0,1,0)), vertex.z-dot(translation,Vec3(-1,0,0)), 1.0);
		std::cout << "vertex Before matrix mul= " << vertex.x << " " << vertex.y << " " << vertex.z << " " << vertex.w << std::endl;
	  Vec4 vertex_eye = vertex.applyMat4(viewMatrices[currentFace]);
		std::cout << "VERTEX EYE = " << vertex_eye.x << " " << vertex_eye.y << " " << vertex_eye.z << " " << vertex_eye.w << std::endl;
		
		transfer.vertices.push_back(vertex_eye);
	}

	//std::cout << transfer.vertices.size() << std::endl;
	//Check if vertices transformed make any sense
	std::cout << "Vertices transformed into eye space" << std::endl;
	for(int i = 0; i < transfer.vertices.size(); ++i){
		Vec4 vertex = transfer.vertices[i];
		std::cout << "vertex = " << vertex.x <<" "<< vertex.y << " " << vertex.z << " " << std::endl;
	}
	
	//2. Perform perspective transformation
	Mat4 perspective = Mat4().initPerspectiveHemicube(1.5, 1000.0);
	for(int i = 0; i < transfer.vertices.size(); ++i){
	  //transfer.vertices[i] = perspective.applyVector(transfer.vertices[i]);
		transfer.vertices[i].w = 1.0;
		std::cout << "Applying Perspective Transformation" << std::endl;
		transfer.vertices[i] = transfer.vertices[i].applyMat4(perspective);
		Vec4 vertex = transfer.vertices[i];
		std::cout << "perspective vertex = " << vertex.x << " " << vertex.y << " " << vertex.z << " " << vertex.w << std::endl;
	}

	std::cout << "Before clipping" << std::endl;
	for(int i = 0; i < transfer.vertices.size(); ++i){
		transfer.vertices[i].z /= 2.0;
		//transfer.vertices[i].w /= 2.0;
		Vec4 vertex = transfer.vertices[i];
		//vertex.w /= 2.0;
		std::cout << "vertex = " << vertex.x << " " << vertex.y << " " << vertex.z << " " << vertex.w << std::endl;
	}
	
	//3. Clip polygon in 4D
	//Polygon out;
	//for(int i = 0; i < transfer.vertices.size(); ++i){
	//std::string name = "P"+std::to_string(i);
		//clippers[0].clipPlane(transfer.vertices[i], out, name);

		//out.vertices.push_back(transfer.vertices[i]);
	//}
	//clippers[0].closeEdge(out);


	//Perform clipping in 3D homogeneous space
	PolygonClass out;
	out.vertices.push_back(transfer.vertices[0]);
	out.vertices.push_back(transfer.vertices[1]);
	out.vertices.push_back(transfer.vertices[2]);
	out.vertices.push_back(transfer.vertices[3]);

	//out.vertices.push_back(Vec4(-4,3,3,3));
	//out.vertices.push_back(Vec4(0,2,3,3));
	//out.vertices.push_back(Vec4(0,-3,3,3));
	//out.vertices.push_back(Vec4(-4,-2,3,3));

	Vec4 vert = out.vertices[3];
	std::cout << "test vertex = " << vert.x << " " << vert.y << " " << vert.z << " " << vert.w << std::endl;

	std::cout << "OUT polygon size = " << out.vertices.size() << std::endl;

	PolygonClass auxiliary;
	//clipTriangle(triangle, out);

	if(clipTriangleAxis(out, 0, auxiliary, false) &&
		 clipTriangleAxis(out, 1, auxiliary, false) &&
		 clipTriangleAxis(out, 2, auxiliary, false)){
		std::cout << "Vertices = " << out.vertices.size() << std::endl;
	}

	//After clipping
	std::cout << "After clipping" << std::endl;
	for(int i = 0; i < out.vertices.size(); ++i){
		Vec4 vertex = out.vertices[i];
		//vertex.w /= 2.0;
		std::cout << "vertex = " << vertex.x <<" "<< vertex.y << " " << vertex.z << " " << vertex.w << std::endl;
	}

	
	//4. Perform homegeneous w division
	for(int i = 0; i < out.vertices.size(); ++i){
		float w = out.vertices[i].w;
		out.vertices[i].x /= w;
		out.vertices[i].y /= w;
		out.vertices[i].z /= (w/2.0);
		//out.vertices[i].z /= w;
	}

	std::cout << "After perspective w division" << std::endl;
	std::cout << "num vertices = " << out.vertices.size() << std::endl;
	for(int i = 0; i < out.vertices.size(); ++i){
		Vec4 vertex = out.vertices[i];
		std::cout << "vertex point render = " << vertex.x << " " << vertex.y << " " << vertex.z << " " << std::endl;
		Vec4 realVertex;
		if(currentFace == FRONT){
			realVertex = vertex;
		}else if(currentFace == LEFT){
			realVertex.x = -vertex.z;
			realVertex.y = vertex.y;
			realVertex.z = 3-vertex.x;
		}
		DebugRenderer::addPoint((Vec3(-1, realVertex.y, realVertex.z)+0.0)*1.0, Vec3(0,1,0));
	}

	//Transfer polygon to be in screen coordinates
	for(int i = 0; i < out.vertices.size(); ++i){
		Vec4 vertex = out.vertices[i];
		std::cout << "vertex to transform to screen coords = " << vertex.x << " " << vertex.y << " " << vertex.z << std::endl;
		out.vertices[i].x = (vertex.x+1.0) * 50.0;
		out.vertices[i].y = (vertex.y+1.0) * 50.0;
		//polygon.vertices[i].z = (vertex.z + 1.0) * 5.0;
		vertex = out.vertices[i];
		std::cout << "vertex transformed to screen coords = " << vertex.x << " " << vertex.y << " " << vertex.z << std::endl;
	}

	std::cout << "transformed vertices" << std::endl;
	for(int i = 0; i < out.vertices.size(); ++i){
		Vec4 vertex = out.vertices[i];
		std::cout << "vertex transformed = " << vertex.x << " " << vertex.y << " " << vertex.z << std::endl;
	}
	
	//5. Fill polygon
	PolygonFill fill;
	fill.fillPolygon(out);
	int canvas[100][100];
	for(int i = 0; i < 100; ++i)
		for(int j = 0; j < 100; ++j)
			canvas[i][j] = 0.0;

	fill.fillCanvas(canvas);
	float size = 1.0f/100.0f;
	for(int y = 0; y < 100; ++y){
		for(int x = 0; x < 100; ++x){
			if(!canvas[y][x])
				continue;
			if(currentFace == FRONT)
				DebugRenderer::addQuad(Vec3(-1+size+(x*size*2),-1+size+(y*size*2),2), Vec3(size,size,0.1f), Vec3(1,1,0));
			else if(currentFace == LEFT)
				DebugRenderer::addQuad(Vec3(-1,-1+size+(y*size*2),1-size+(x*size*2)), Vec3(size,size,0.1f), Vec3(1,1,0));
		}
	}
}

bool clipTriangleAxis(PolygonClass& triangle, int c, PolygonClass& auxiliary, bool halfAxis){
	clipTriangleComponent(triangle, c, 1.0, auxiliary, false);
	triangle.vertices.clear();

	if(auxiliary.vertices.size() == 0)
		return false;

	if(halfAxis)
		clipTriangleComponent(auxiliary, c, 1.0, triangle, true);
	else
		clipTriangleComponent(auxiliary, c, -1.0, triangle, false);
	auxiliary.vertices.clear();

	return !triangle.vertices.empty();
	//return true;
}

bool clipTriangleHalfAxis(PolygonClass& triangle, int c, PolygonClass& auxiliary){
	clipTriangleComponent(triangle, c, 1.0, auxiliary, false);
	return true;
}

void clipTriangleComponent(PolygonClass triangle, int c, float compFactor, PolygonClass& out, bool wzero){
	std::cout << "Clip component " << compFactor << c << std::endl;
	
	Vec4 previousVertex = triangle.vertices.back();
	float previousComponent = previousVertex[c] * compFactor;
	bool previousInside;
	if(wzero)
		previousInside = previousComponent >= 0;
	else
		previousInside = previousComponent <= previousVertex.w;

	std::cout << previousComponent << " " << previousVertex.w << std::endl;
	
	for(int i = 0; i < triangle.vertices.size(); ++i){
		Vec4 currentVertex = triangle.vertices[i];
		float currentComponent = currentVertex[c] * compFactor;
		bool currentInside;
		if(wzero)
			currentInside = currentComponent >= 0;
		else
			currentInside = currentComponent <= currentVertex.w;

		std::cout << currentComponent << " " << currentVertex.w << std::endl;
		
		if(currentInside ^ previousInside){
			//Linear interpolation between the two vertices
			float alpha;
			if(wzero)
				alpha = -previousComponent / (-previousComponent + currentComponent);
			else
				alpha = (previousVertex.w - previousComponent)
				/ ((previousVertex.w - previousComponent)-(currentVertex.w - currentComponent));
			Vec4 newVertex = lerp(previousVertex, currentVertex, alpha);
			out.vertices.push_back(newVertex);

			std::cout << "lerped" << std::endl;
		}
		
		if(currentInside)
			out.vertices.push_back(currentVertex);
		
		previousVertex = currentVertex;
		previousComponent = currentComponent;
		previousInside = currentInside;
	}
}

Vec4 lerp(Vec4 A, Vec4 B, float alpha){
	return A*(1.0 - alpha) + B*alpha;
}

float Hemicube::getTopFactor(int row, int col){
	if(row >= arrayDim)
		row -= arrayDim;
	else
		row = arrayDim - row - 1;

	if(col >= arrayDim)
		col -= arrayDim;
	else
		col = arrayDim - col - 1;

	return top_array[row][col];
}

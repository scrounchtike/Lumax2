
#ifndef HEMICUBE_HPP
#define HEMICUBE_HPP

#include "../math.hpp"

#include "polygon_clip.hpp"
#include "Polygon_Fill.hpp"

class Hemicube{
public:
	Hemicube();
	void transformPolygon(PolygonClass& polygon);
	
	ClipPlane clippers[5];
	Mat4 viewMatrices[5];

	static const int arrayRes = 100;
	static const int arrayDim = arrayRes / 2;

	static float top_array[arrayDim][arrayDim];
	float getTopFactor(int row, int col);
};

#endif

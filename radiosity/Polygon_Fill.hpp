
#ifndef POLYGON_FILL_HPP
#define POLYGON_FILL_HPP

#include "../math.hpp"
#include "../radiosity/polygon_clip.hpp"

struct ScanInfo{
	float x, z;
	bool init = false;
	ScanInfo(){
		x = z = 0;
		init = false;
	}
	ScanInfo(float xval, float zval){
		x = xval; z = zval;
		init = true;
	}
};

class PolygonFill{
public:
	PolygonFill();

	void fillPolygon(PolygonClass& out);
	void scanEdge(Vec3 S, Vec3 E);

	void fillCanvas(int canvas[100][100]);
private:
	int dimension = 100;
	ScanInfo intersections[100][2];
	int ymax;
	int ymin;

	void addScanIntersection(int y, float sx, float sz);
};

#endif

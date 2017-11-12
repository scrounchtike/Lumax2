
#ifndef DOOM_PLANE_HPP
#define DOOM_PLANE_HPP

#include "../math.hpp"
#include "../RL/Model3D.hpp"

#include <algorithm>

class DOOM_Plane{
public:
	Vec3 normal;
	Vec3 start;
	Vec3 end;
	
	DOOM_Plane(Vec3 pos1, Vec3 pos2, Vec3 pos3, Vec3 pos4, Vec3 normal, Vec3 tangent);

	void bindForRender();
	void render();
	void unbindForRender();

	void fullRender();
private:
	Model3D* model;
};

#endif

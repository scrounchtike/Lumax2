
#ifndef D_WALL_HPP
#define D_WALL_HPP

#include "../math.hpp"
#include "../rendering/Model3D.hpp"

class D_wall{
public:
	D_wall(Vec2 pos1, Vec2 pos2, Vec2 normal);

	void bindForRender();
	void render();
	void unbindForRender();
	
	void fullRender();
private:
	Model3D* wall_model;
};

#endif

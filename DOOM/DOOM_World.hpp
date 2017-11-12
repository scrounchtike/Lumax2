
#ifndef DOOM_WORLD_HPP
#define DOOM_WORLD_HPP

#include <vector>
#include "../math.hpp"
#include "../rendering/Texture.hpp"

#include "DOOM_plane.hpp"
//#include "DOOM_Player.hpp"

struct DOOM_Flat{
	float stepHeight;
	DOOM_Plane* plane;
	Texture* texture;
	DOOM_Flat(DOOM_Plane* plane, Texture* texture){
		this->plane = plane; this->texture = texture;
		this->stepHeight = -1.0f;
	}
};

class DOOM_World{
public:
	DOOM_World();

	void setActiveTexture(Texture* texture){ activeTexture = texture; }
	
	void addWall(Vec2 edge1, Vec2 edge2, Vec2 normal);
	void addWall(Vec3 edge1, Vec3 edge2, Vec3 normal);
	void addStep(Vec3 edge1, Vec3 edge2, Vec3 normal);
	void addFloor(Vec2 start, Vec2 end);
	void addFloor(Vec2 start, Vec2 end, float height);
	void addCeiling(Vec3 pos1, Vec3 pos2, Vec3 pos3, Vec3 pos4);
	void addCeiling(Vec2 start, Vec2 end, float height);

	void checkHeightOnFloor(Vec3 oldPos, Vec3& posDelta);

	void checkCollisionWalls(Vec3 oldPos, Vec3& posDelta);
	bool checkCollisionWall(DOOM_Flat& wall, Vec3 newPos, Vec3& posDelta);

	void input();
	void update();
	void render();
private:
	Texture* activeTexture;
	
	std::vector<DOOM_Flat> walls;
	std::vector<DOOM_Flat> floors;
	std::vector<DOOM_Flat> ceils;
};

#endif

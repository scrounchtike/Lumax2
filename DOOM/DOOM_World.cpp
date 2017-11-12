
#include "DOOM_World.hpp"

DOOM_World::DOOM_World(){
	
}

void DOOM_World::addWall(Vec2 edge1, Vec2 edge2, Vec2 normal){
	walls.push_back(DOOM_Flat(new DOOM_Plane(Vec3(edge1.x, 1.5f, edge1.y), Vec3(edge1.x, 0, edge1.y), Vec3(edge2.x, 0, edge2.y), Vec3(edge2.x, 1.5f, edge2.y), Vec3(normal.x, 0, normal.y), Vec3(0,1,0)), activeTexture));
}

void DOOM_World::addWall(Vec3 edge1, Vec3 edge2, Vec3 normal){
	walls.push_back(DOOM_Flat(new DOOM_Plane(Vec3(edge1.x, edge2.y, edge1.z), Vec3(edge1.x, edge1.y, edge1.z), Vec3(edge2.x, edge1.y, edge2.z), Vec3(edge2.x, edge2.y, edge2.z), Vec3(normal.x, normal.y, normal.z), Vec3(0,1,0)), activeTexture));
}

void DOOM_World::addStep(Vec3 edge1, Vec3 edge2, Vec3 normal){
	walls.push_back(DOOM_Flat(new DOOM_Plane(Vec3(edge1.x, edge2.y, edge1.z), Vec3(edge1.x, edge1.y, edge1.z), Vec3(edge2.x, edge1.y, edge2.z), Vec3(edge2.x, edge2.y, edge2.z), Vec3(normal.x, normal.y, normal.z), Vec3(0,1,0)), activeTexture));
	if(edge1.y > edge2.y)
		walls.back().stepHeight = edge1.y - edge2.y;
	else
		walls.back().stepHeight = edge2.y - edge1.y;
}

void DOOM_World::addFloor(Vec2 start, Vec2 end){
	floors.push_back(DOOM_Flat(new DOOM_Plane(Vec3(start.x, 0, start.y), Vec3(start.x, 0, end.y), Vec3(end.x, 0, end.y), Vec3(end.x, 0, start.y), Vec3(0,1,0), Vec3(0,0,-1)), activeTexture));
}

void DOOM_World::addFloor(Vec2 start, Vec2 end, float height){
	floors.push_back(DOOM_Flat(new DOOM_Plane(Vec3(start.x, height, start.y), Vec3(start.x, height, end.y), Vec3(end.x, height, end.y), Vec3(end.x, height, start.y), Vec3(0,1,0), Vec3(0,0,-1)), activeTexture));
}

void DOOM_World::addCeiling(Vec3 pos1, Vec3 pos2, Vec3 pos3, Vec3 pos4){
	Vec3 normal = cross(pos4-pos1, pos2-pos1);
	Vec3 tangent = pos4-pos1;
	ceils.push_back(DOOM_Flat(new DOOM_Plane(pos1, pos2, pos3, pos4, normal, tangent), activeTexture));
}

void DOOM_World::addCeiling(Vec2 start, Vec2 end, float height){
	ceils.push_back(DOOM_Flat(new DOOM_Plane(Vec3(start.x, height, start.y), Vec3(start.x, height, end.y), Vec3(end.x, height, end.y), Vec3(end.x, height, start.y), Vec3(0,-1,0), Vec3(0,0,-1)), activeTexture));
}

void DOOM_World::checkHeightOnFloor(Vec3 oldPos, Vec3 &posDelta){
	Vec3 newPos = oldPos + posDelta;

	float playerHeight = 0.6f;
	float dist = 0.155f;
	Vec3 player_points[4] = {newPos+Vec3(-dist,0,-dist), newPos+Vec3(dist,0,-dist), newPos+Vec3(dist,0,dist), newPos+Vec3(-dist,0,dist)};
	
	float currentHeight = oldPos.y - playerHeight;
	bool changedHeight = false;
	for(int i = 0; i < floors.size(); ++i){
		Vec3 start = floors[i].plane->start;
		Vec3 end = floors[i].plane->end;
		bool allPoints = true;
		bool atleastOnePoint = false;
		
		for(int j = 0; j < 4; ++j){
			if(player_points[j].x > end.x || player_points[j].x < start.x){
				allPoints = false;
				continue;
			}
			if(player_points[j].z > end.z || player_points[j].z < start.z){
				allPoints = false;
				continue;
			}
			atleastOnePoint = true;
		}
		if(atleastOnePoint && start.y > currentHeight){
			currentHeight = start.y;
			changedHeight = true;
		}
		if(allPoints && start.y < currentHeight){
			currentHeight = start.y;
			changedHeight = true;
		}
	}
	if(changedHeight)
		posDelta.y = currentHeight - (oldPos.y - playerHeight);
}

void DOOM_World::checkCollisionWalls(Vec3 oldPos, Vec3& posDelta){
	Vec3 newPos = oldPos + posDelta;
	
	//Check collisions
	//TODO: Get rid of the need of a normal?
	//Unless its for performance sake (only check if we are facing a wall/plane)
	for(int i = 0; i < walls.size(); ++i){
		bool collided = checkCollisionWall(walls[i], newPos, posDelta);
	}
}

bool DOOM_World::checkCollisionWall(DOOM_Flat &wall, Vec3 newPos, Vec3 &posDelta){
	//Vec3 normal = Vec3(1,0,0);
	Vec3 normal = wall.plane->normal;
	Vec3 extents = ((wall.plane->end - wall.plane->start) / 2.0);
	float extent = sqrt(extents.x*extents.x + extents.z*extents.z);
	float D = dot(normal, wall.plane->start);
	
	//TODO: Player AABB dependent on player class
	float dist = 0.155f;
	Vec3 player_points[4] = {newPos+Vec3(-dist,0,-dist), newPos+Vec3(dist,0,-dist), newPos+Vec3(dist,0,dist), newPos+Vec3(-dist,0,dist)};
	
	bool colliding = false;
	for(int i = 0; i < 4; ++i){
		float inside = dot(normal, player_points[i]);
		if(inside < D){
			if(dot(normal, player_points[i]-posDelta) < D)
				continue;
			//this point is colliding
			//std::cout << "inside" << std::endl;
			colliding = true;
			break;
		}
	}
	if(colliding){
		Vec3 tangent = cross(Vec3(0,1,0), normal).normalize();
		Vec3 edge1 = wall.plane->end;
		Vec3 edge2 = wall.plane->start;
		
		Vec3 normal1 = -tangent;
		Vec3 normal2 = tangent;
		float D1 = dot(normal1, edge1);
		float D2 = dot(normal2, edge2);

		bool atleastOneInside = false;
		for(int i = 0; i < 4; ++i){
			float dot1 = dot(normal1, player_points[i]-posDelta);
			float dot2 = dot(normal2, player_points[i]-posDelta);
			if(dot1 > D1 && dot2 > D2){
				atleastOneInside = true;
				break;
			}
		}
		if(atleastOneInside){
			//Check if wall is a step
			if(wall.stepHeight > 0){
				//posDelta += Vec3(0, wall.stepHeight, 0);
			}else
				posDelta = tangent * dot(tangent, posDelta);
		}
	}
	return colliding;
}

void DOOM_World::input(){
	
}

void DOOM_World::update(){
	
}

void DOOM_World::render(){
	//render walls
	for(int i = 0; i < walls.size(); ++i){
		walls[i].texture->bind();
		walls[i].plane->fullRender();
	}
	//render floors
	for(int i = 0; i < floors.size(); ++i){
		floors[i].texture->bind();
		floors[i].plane->fullRender();
	}
	//render ceiling
	for(int i = 0; i < ceils.size(); ++i){
		ceils[i].texture->bind();
		ceils[i].plane->fullRender();
	}
}

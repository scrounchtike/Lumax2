
#ifndef DOOM_PLAYER_HPP
#define DOOM_PLAYER_HPP

#include "../math.hpp"
#include "../rendering/Camera.hpp"
#include "../RL/Model2D.hpp"
#include "../rendering/Texture.hpp"
#include "../rendering/Shader.hpp"

#include "DOOM_World.hpp"

class DOOM_Player{
public:
	DOOM_Player(Camera* camera);

	void input(DOOM_World* world);
	void update();
	void render();
	Camera* getCamera(){ return camera; }
private:
	Camera* camera;
	
	Model2D* model_player;
	Texture* gun;
	Texture* gun_firing;

	Shader* shader;
};

#endif

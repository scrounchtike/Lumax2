
#ifndef CAMERA_HPP
#define CAMERA_HPP

#include "../math.hpp"
#include "../main.hpp"

#include "../RAL/Window.hpp"

class Camera{
public:
	Camera(Window* window, Vec3 position);

	void input();
	void input(Vec3 posDelta);
	void update();

	Mat4& getProjection(){ return projection_matrix; }
	Mat4& getView(){ return view_matrix; }

	Vec3& getPosition(){ return position; }
	Vec3& getWalk(){ return walk; }
	Vec3& getForward(){ return forward; }
	Vec3& getUp(){ return up; }
	const Vec3& getYAxis(){ return yAxis; }
	Vec3& getRight(){ return right; }

	void setSpeed(float speed){ this->speed = speed; }
	float getSpeed() const{ return speed; }

	void setWindow(Window* window) { this->window = window; }
	Window* getWindow() { return window; }
private:
	// Necessary for input and cursor control
	Window* window;

	void rotateX(float theta);
	void rotateY(float theta);

	float speed;

	static const Vec3 yAxis;

	bool cursorLocked;
	Vec3 position;
	Vec3 forward;
	Vec3 walk;
	Vec3 up;
	Vec3 right;

	Mat4 projection_matrix;
	Mat4 view_matrix;
	
	const float FOV = 70.0f;
	const float zNear = 0.1f;
	const float zFar = 1000.0f;
};

#endif


#include "Camera.hpp"

// TODO: Remove since this class is independent of OS
#include <windows.h>
#include <string>

const Vec3 Camera::yAxis(0,1,0);

Camera::Camera(Window* window, Vec3 eye) : window(window), position(eye){
	// TODO: Implement different looking directions for D3D11 and OpenGL
	forward = Vec3(0,0,1);
	up = Vec3(0,1,0);
	right = Vec3(1,0,0);
	walk = Vec3(0,0,1);

	cursorLocked = false;

	speed = 0.3f;

	//Projection matrix
	projection_matrix.initProjectionDX11(FOV, 600.0/800.0, zNear, zFar);
	//projection_matrix.initPerspectiveHemicube(0.1f, 1000.0);
	view_matrix.initCamera(forward, up, right);
}

void Camera::input(){
	Vec3 posDelta = Vec3(0, 0, 0);

	if(wasKeyPressed(DIK_W))
		posDelta += (walk*speed);
	if(wasKeyPressed(DIK_S))
		posDelta += -(walk*speed);
	if(wasKeyPressed(DIK_A))
		posDelta += -(right*speed);
	if(wasKeyPressed(DIK_D))
		posDelta += (right*speed);
	if(wasKeyPressed(DIK_SPACE))
		posDelta += (yAxis*speed);
	if(wasKeyPressed(DIK_LSHIFT))
		posDelta += -(yAxis*speed);

	input(posDelta);
}

void Camera::input(Vec3 posDelta){
	position += posDelta;

	//If cursor is not locked and click on screen, then lock cursor.
	if(!cursorLocked && wasButtonJustPressed(0)){
		cursorLocked = true;
		window->showCursor(false);
		window->setCursorPosition(window->getWidth()/2.0, window->getHeight()/2.0);
	}
	//If in cursor locked mode and press ESC then exit mouse lock mode.
	if(cursorLocked && wasKeyJustPressed(DIK_ESCAPE)){
		cursorLocked = false;
		window->showCursor(true);
		window->setCursorPosition(window->getWidth() /2.0, window->getHeight()/2.0);
	}
	//If cursor is not locked then return
	if(!cursorLocked)
		return;

	int cursorX, cursorY;
	getMousePosition(cursorX, cursorY);
	OutputDebugString((std::to_string(cursorX) + "\n").c_str());
	float deltaX = cursorX - (window->getWidth() / 2.0);
	float deltaY = cursorY - (window->getHeight() / 2.0);

	//Center cursor
	window->setCursorPosition(window->getWidth()/2.0, window->getHeight()/2.0);

	float sensitivity = 0.003f;
	if(deltaX)
		rotateY(sensitivity * deltaX);
	if(deltaY)
		rotateX(sensitivity * deltaY);
}

void Camera::update(){
	//OutputDebugString((LPCSTR)((std::to_string(position.z)+"\n").c_str()));
	Mat4 camera_movement = Mat4().initTranslation(-position.x, -position.y, -position.z);
	view_matrix.initCamera(forward, up, right);
	view_matrix.mul(camera_movement);
}

void Camera::rotateX(float theta){
	//TODO: Figure out if Haxis is the right vector in this situation
	Vec3 Haxis = cross(yAxis, forward);
	Haxis.normalize();
	//Apparently it is..
	Haxis = right;
	
	forward = rotate(forward, Haxis, theta);
	forward.normalize();

	up = cross(forward, Haxis);
	up.normalize();

	//Right did not change
	right = cross(up, forward);
	right.normalize();
}

void Camera::rotateY(float theta){
	Vec3 Haxis = cross(yAxis, forward);
	Haxis.normalize();
	
	forward = rotate(forward, yAxis, theta);
	forward.normalize();

	walk = Vec3(forward.x, 0, forward.z);
	walk.normalize();

	up = cross(forward, Haxis);
	up.normalize();

	right = cross(up, forward);
	right.normalize();
	
	//Update vectors with rotation
	//forward = rotate(forward, yAxis, theta);
  //forward.normalize();

	//Right vector becomes cross product between yAxis and forward
	//right = vec3::cross(up, forward);
	//right.normalize();
}

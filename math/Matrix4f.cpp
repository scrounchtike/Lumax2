
#include <string.h>

#include "Matrix4f.hpp"

#define M_PI 3.1415927

Matrix4f::Matrix4f(){
	initIdentity();
}

Matrix4f& Matrix4f::initIdentity(){
	m[0][0] = 1;    m[0][1] = 0;    m[0][2] = 0;    m[0][3] = 0;
	m[1][0] = 0;    m[1][1] = 1;    m[1][2] = 0;    m[1][3] = 0;
	m[2][0] = 0;    m[2][1] = 0;    m[2][2] = 1;    m[2][3] = 0;
	m[3][0] = 0;    m[3][1] = 0;    m[3][2] = 0;    m[3][3] = 1;
	return *this;
}

Matrix4f& Matrix4f::initTranslation(float x, float y, float z){
	m[0][0] = 1;    m[0][1] = 0;    m[0][2] = 0;    m[0][3] = x;
	m[1][0] = 0;    m[1][1] = 1;    m[1][2] = 0;    m[1][3] = y;
	m[2][0] = 0;    m[2][1] = 0;    m[2][2] = 1;    m[2][3] = z;
	m[3][0] = 0;    m[3][1] = 0;    m[3][2] = 0;    m[3][3] = 1;
	return *this;
}

Matrix4f& Matrix4f::initTranslation(Vec3 translation){
	return initTranslation(translation.x, translation.y, translation.z);
}

Matrix4f& Matrix4f::initRotation(float x, float y, float z){
	Matrix4f rotX = Matrix4f().initRotationX(x);
	Matrix4f rotY = Matrix4f().initRotationY(y);
	Matrix4f rotZ = Matrix4f().initRotationZ(z);
	return rotX.mul(rotY.mul(rotZ));
}

Matrix4f& Matrix4f::initRotation(Vec3 rotation){
	return initRotation(rotation.x, rotation.y, rotation.z);
}

Matrix4f& Matrix4f::initRotationX(float theta){
	float cosTheta = cos(theta);
	float sinTheta = sin(theta);
	m[0][0] = 1;    m[0][1] = 0;           m[0][2] = 0;            m[0][3] = 0;
	m[1][0] = 0;    m[1][1] = cosTheta;    m[1][2] = -sinTheta;    m[1][3] = 0;
	m[2][0] = 0;    m[2][1] = sinTheta;    m[2][2] = cosTheta;     m[2][3] = 0;
	m[3][0] = 0;    m[3][1] = 0;           m[3][2] = 0;            m[3][3] = 1;
	return *this;
}

Matrix4f& Matrix4f::initRotationY(float theta){
	float cosTheta = cos(theta);
	float sinTheta = sin(theta);
	m[0][0] = cosTheta;    m[0][1] = 0;    m[0][2] = -sinTheta;    m[0][3] = 0;
	m[1][0] = 0;           m[1][1] = 1;    m[1][2] = 0;            m[1][3] = 0;
	m[2][0] = sinTheta;    m[2][1] = 0;    m[2][2] = cosTheta;     m[2][3] = 0;
	m[3][0] = 0;           m[3][1] = 0;    m[3][2] = 0;            m[3][3] = 1;
	return *this;
}

Matrix4f& Matrix4f::initRotationZ(float theta){
	float cosTheta = cos(theta);
	float sinTheta = sin(theta);
	m[0][0] = cosTheta;    m[0][1] = -sinTheta;    m[0][2] = 0;    m[0][3] = 0;
	m[1][0] = sinTheta;    m[1][1] = cosTheta;     m[1][2] = 0;    m[1][3] = 0;
	m[2][0] = 0;           m[2][1] = 0;            m[2][2] = 1;    m[2][3] = 0;
	m[3][0] = 0;           m[3][1] = 0;            m[3][2] = 0;    m[3][3] = 1;
	return *this;
}

Matrix4f& Matrix4f::initScale(float x, float y, float z){
	m[0][0] = x;    m[0][1] = 0;    m[0][2] = 0;    m[0][3] = 0;
	m[1][0] = 0;    m[1][1] = y;    m[1][2] = 0;    m[1][3] = 0;
	m[2][0] = 0;    m[2][1] = 0;    m[2][2] = z;    m[2][3] = 0;
	m[3][0] = 0;    m[3][1] = 0;    m[3][2] = 0;    m[3][3] = 1;
	return *this;
}

Matrix4f& Matrix4f::initScale(Vec3 scale){
	return initScale(scale.x, scale.y, scale.z);
}

Matrix4f& Matrix4f::initCamera(Vec3 forward, Vec3 up, Vec3 right){
	m[0][0] = right.x;    m[0][1] = right.y;    m[0][2] = right.z;    m[0][3] = 0;
	m[1][0] = up.x;       m[1][1] = up.y;       m[1][2] = up.z;       m[1][3] = 0;
	m[2][0] = forward.x;  m[2][1] = forward.y;  m[2][2] = forward.z;  m[2][3] = 0;
	m[3][0] = 0;          m[3][1] = 0;          m[3][2] = 0;          m[3][3] = 1;
	return *this;
}

Matrix4f& Matrix4f::initProjection(float FOV, float zNear, float zFar){
	float tanHalfFOV = tan((FOV*M_PI/180.0)/2.0);
	float ar = 600.0f/800.0f;
	float zRange = zFar - zNear;
	m[0][0] = ar/tanHalfFOV;        m[0][1] = 0;     m[0][2] = 0;    m[0][3] = 0;
	m[1][0] = 0;    m[1][1] = 1.0/tanHalfFOV;        m[1][2] = 0;    m[1][3] = 0;
	m[2][0] = 0;    m[2][1] = 0;    m[2][2] = (-zNear-zFar)/zRange;  m[2][3] = -2.0*zNear*zFar/zRange;
	m[3][0] = 0;    m[3][1] = 0;    m[3][2] = -1;    m[3][3] = 0;
	return *this;
}

Matrix4f& Matrix4f::initProjectionDX11(float FOV, float ar, float zNear, float zFar) {
	float tanHalfFOV = tan((FOV*M_PI / 180.0) / 2.0);
	float zRange = zFar - zNear;
	m[0][0] = ar / tanHalfFOV;        m[0][1] = 0;     m[0][2] = 0;    m[0][3] = 0;
	m[1][0] = 0;    m[1][1] = 1.0 / tanHalfFOV;        m[1][2] = 0;    m[1][3] = 0;
	m[2][0] = 0;    m[2][1] = 0;    m[2][2] = zFar / zRange;  m[2][3] = -zFar * zNear / zRange;
	m[3][0] = 0;    m[3][1] = 0;    m[3][2] = 1;     m[3][3] = 0;

	//m[0][0] = 1;    m[0][1] = 0;    m[0][2] = 0;    m[0][3] = 0;
	//m[1][0] = 0;    m[1][1] = 1;    m[1][2] = 0;    m[1][3] = 0;
	//m[2][0] = 0;    m[2][1] = 0;	m[2][2] = 1;	m[2][3] = 0;
	//m[3][0] = 0;    m[3][1] = 0;    m[3][2] = 1;    m[3][3] = 0;
	return *this;
}

Matrix4f& Matrix4f::initPerspectiveHemicube(float zNear, float zFar){
	float zRange = zFar - zNear;
	float a = (zFar + zNear)/zRange;
	float b = (2.0*zNear*zFar)/zRange;
	m[0][0] = 1;    m[0][1] = 0;    m[0][2] = 0;    m[0][3] = 0;
	m[1][0] = 0;    m[1][1] = 1;    m[1][2] = 0;    m[1][3] = 0;
	m[2][0] = 0;    m[2][1] = 0;    m[2][2] = 1;    m[2][3] = 3;
	m[3][0] = 0;    m[3][1] = 0;    m[3][2] = 1;   m[3][3] = 0;
	return *this;
}

Matrix4f& Matrix4f::transpose() {
	float temp[4][4];
	memcpy(temp, m, sizeof(float) * 16);
	for (int i = 0; i < 4; ++i) {
		m[i][0] = temp[0][i];
		m[i][1] = temp[1][i];
		m[i][2] = temp[2][i];
		m[i][3] = temp[3][i];
	}
	return *this;
}

// TODO: Decompose transformation matrices into their main components
Vec3 Matrix4f::getTranslationComponent() {
	return Vec3(m[0][3], m[1][3], m[2][3]);
}

Vec3 Matrix4f::getRotationComponent() {
	return Vec3(0, 0, 0);
}

Quaternion Matrix4f::getRotationQuaternionComponent() {
	return Quaternion(0, 0, 0, 1);
}

Vec3 Matrix4f::getScaleComponent(){
	return Vec3(1, 1, 1);
}
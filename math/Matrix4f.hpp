
#ifndef MATRIX_4F_HPP
#define MATRIX_4F_HPP

#include "Vector3.hpp"
#include "Quaternion.hpp"

#define PI 3.1415926535

class Matrix4f{
public:
	Matrix4f();
	Matrix4f& initIdentity();
	Matrix4f& initTranslation(float x, float y, float z);
	Matrix4f& initTranslation(Vec3 translation);

	Matrix4f& initCamera(Vec3 forward, Vec3 up, Vec3 right);

	Matrix4f& initProjection(float FOV, float zNear, float zFar);
	Matrix4f& initProjectionDX11(float FOV, float ar, float zNear, float zFar);

	Matrix4f& initRotation(float x, float y, float z);
	Matrix4f& initRotation(Vec3 rotation);
	Matrix4f& initRotationX(float angle);
	Matrix4f& initRotationY(float angle);
	Matrix4f& initRotationZ(float angle);

	Matrix4f& initScale(float x, float y, float z);
	Matrix4f& initScale(Vec3 scale);

	Matrix4f& initPerspectiveHemicube(float zNear, float zFar);

	Matrix4f& transpose();

	Vec3 getTranslationComponent();
	Vec3 getRotationComponent();
	Quaternion getRotationQuaternionComponent();
	Vec3 getScaleComponent();

	float get(int row, int col){ return m[row][col]; }
	void set(int row, int col, float value){ m[row][col] = value; }
	//float* getHeadPointer(){ return &m[0][0]; }
	float* getHeadPointer(){
		float* matrix = new float[16];
		for(int i = 0; i < 4; ++i){
			for(int j = 0; j < 4; ++j){
				matrix[i*4+j] = m[i][j];
			}
		}
		return matrix;
	}

	Matrix4f& mul(Matrix4f& m2){
		float row[4];
		for(int i = 0; i < 4; ++i){
			for(int j = 0; j < 4; ++j){
				row[j] = m[i][0]*m2.get(0,j) + m[i][1]*m2.get(1,j) + m[i][2]*m2.get(2,j) + m[i][3]*m2.get(3,j);
			}
			for(int j = 0; j < 4; ++j)
				m[i][j] = row[j];
		}
		return *this;
	}
	static Matrix4f mul(Matrix4f& m1, Matrix4f& m2){
		Matrix4f matrix;
		for(int i = 0; i < 4; ++i){
			for(int j = 0; j < 4; ++j){
				matrix.set(i, j,  m1.get(i, 0)*m2.get(0,j) + m1.get(i, 1)*m2.get(1,j) + m1.get(i, 2)*m2.get(2,j) + m1.get(i, 3)*m2.get(3,j));
			}
		}
		return matrix;
	}
	
private:
	float m[4][4];
};

typedef Matrix4f Mat4;

#endif

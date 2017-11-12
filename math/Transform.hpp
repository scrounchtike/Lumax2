
#pragma once

#ifndef TRANSFORM_HPP
#define TRANSFORM_HPP

#include "Vector3.hpp"
#include "Matrix4f.hpp"
#include "Quaternion.hpp"

class Transform {
public:
	Transform();
	~Transform();

	void initialize(Vec3 translation = Vec3(0, 0, 0), Vec3 rotation = Vec3(0, 0, 0), Vec3 scale = Vec3(1, 1, 1));
	void initialize(Vec3 translation = Vec3(0, 0, 0), Quaternion rotation = Quaternion(0,0,0,1), Vec3 scale = Vec3(1, 1, 1));

	void initialize(Vec3 translation = Vec3(0, 0, 0), Vec3 rotation = Vec3(0, 0, 0), Vec3 scale = Vec3(1, 1, 1), Vec3 pivot = Vec3(0, 0, 0), Vec3 pivotRotation = Vec3(0, 0, 0));
	void initialize(Vec3 translation = Vec3(0, 0, 0), Quaternion rotation = Quaternion(0, 0, 0, 1), Vec3 scale = Vec3(1, 1, 1), Vec3 pivot = Vec3(0, 0, 0), Vec3 pivotRotation = Vec3(0, 0, 0));

	void initializeTranslationOnly(Vec3 translation);
	void initializeRotationOnly(Vec3 rotation);
	void initializeRotationOnly(Quaternion rotation);
	void initializeScaleOnly(Vec3 scale);
	void initializePivotOnly(Vec3 pivot, Vec3 rotation);

	void initializeTranslationRotation(Vec3 translation, Vec3 rotation);
	void initializeTranslationRotation(Vec3 translation, Quaternion rotation);
	void initializeTranslationScale(Vec3 translation, Vec3 scale);
	void initializeTranslationPivot(Vec3 translation, Vec3 pivot, Vec3 pivotRotation);
	void initializeRotationScale(Vec3 rotation, Vec3 scale);
	void initializeRotationScale(Quaternion rotation, Vec3 scale);
	void initializeRotationPivot(Vec3 rotation, Vec3 pivot, Vec3 pivotRotation);
	void initializeRotationPivot(Quaternion rotation, Vec3 pivot, Vec3 pivotRotation);
	void initializeScalePivot(Vec3 scale, Vec3 pivot, Vec3 pivotRotation);
	
	void setTranslation(Vec3 translation);
	void setRotation(Vec3 rotation);
	void setRotation(Quaternion rotation);
	void setScale(Vec3 scale);
	void setPivot(Vec3 pivot, Vec3 pivotRotation);
	void setPivotPoint(Vec3 pivot);
	void setPivotRotation(Vec3 pivotRotation);

	void calculateTransformation();

	// TODO: Add methods to decompose matrices into their components
	Vec3 getTranslation();
	Vec3 getRotation();
	Vec3 getScale();

	Mat4 getTranslationMatrix() { return translationMatrix; }
	Mat4 getRotationMatrix() { return rotationMatrix; }
	Mat4 getScaleMatrix() { return scaleMatrix; }
	Mat4 getPivotTranslationMatrix() { return pivotTranslationMatrix; }
	Mat4 getPivotRotationMatrix() { return pivotRotationMatrix; }

	Mat4 getTransformation() { return transformation; }
private:
	void setMatrices(Vec3 translation = Vec3(0, 0, 0), Vec3 rotation = Vec3(0, 0, 0), Vec3 scale = Vec3(1, 1, 1), Vec3 pivot = Vec3(0, 0, 0), Vec3 pivotRotation = Vec3(0, 0, 0));
	void setMatricesQuaternion(Vec3 translation = Vec3(0, 0, 0), Quaternion rotation = Quaternion(0, 0, 0, 1), Vec3 scale = Vec3(1, 1, 1), Vec3 pivot = Vec3(0, 0, 0), Vec3 pivotRotation = Vec3(0, 0, 0));

	Mat4 translationMatrix;
	Mat4 rotationMatrix;
	Mat4 scaleMatrix;
	Mat4 pivotTranslationMatrix;
	Mat4 pivotRotationMatrix;

	Mat4 transformation;
};

#endif
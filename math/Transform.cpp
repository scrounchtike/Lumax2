
#include "Transform.hpp"

Transform::Transform() {
	setMatrices();
}

void Transform::initialize(Vec3 translation, Vec3 rotation, Vec3 scale, Vec3 pivot, Vec3 pivotRotation) {
	setMatrices(translation, rotation, scale, pivot, pivotRotation);
}

void Transform::initialize(Vec3 translation, Quaternion rotation, Vec3 scale, Vec3 pivot, Vec3 pivotRotation) {
	setMatricesQuaternion(translation, rotation, scale, pivot, pivotRotation);
}

void Transform::initializeTranslationOnly(Vec3 translation) {
	setMatrices(translation);
}

void Transform::initializeRotationOnly(Vec3 rotation) {
	setMatrices(Vec3(0, 0, 0), rotation);
}

void Transform::initializeRotationOnly(Quaternion rotation) {
	setMatricesQuaternion(Vec3(0, 0, 0), rotation);
}

void Transform::initializeScaleOnly(Vec3 scale) {
	setMatrices(Vec3(0, 0, 0), Vec3(0, 0, 0), scale);
}

void Transform::initializePivotOnly(Vec3 pivot, Vec3 pivotRotation) {
	setMatrices(Vec3(0, 0, 0), Vec3(0, 0, 0), Vec3(1, 1, 1), pivot, pivotRotation);
}

void Transform::initializeTranslationRotation(Vec3 translation, Vec3 rotation) {
	setMatrices(translation, rotation);
}

void Transform::initializeTranslationRotation(Vec3 translation, Quaternion rotation) {
	setMatricesQuaternion(translation, rotation);
}

void Transform::initializeTranslationPivot(Vec3 translation, Vec3 pivot, Vec3 pivotRotation) {
	setMatrices(translation, Vec3(0, 0, 0), Vec3(1, 1, 1), pivot, pivotRotation);
}

void Transform::initializeTranslationScale(Vec3 translation, Vec3 scale) {
	setMatrices(translation, Vec3(0, 0, 0), scale);
}

void Transform::initializeRotationScale(Vec3 rotation, Vec3 scale) {
	setMatrices(Vec3(0, 0, 0), rotation, scale);
}

void Transform::initializeRotationScale(Quaternion rotation, Vec3 scale) {
	setMatricesQuaternion(Vec3(0, 0, 0), rotation, scale);
}

void Transform::initializeRotationPivot(Vec3 rotation, Vec3 pivot, Vec3 pivotRotation) {
	setMatrices(Vec3(0, 0, 0), rotation, Vec3(1, 1, 1), pivot, pivotRotation);
}

void Transform::initializeRotationPivot(Quaternion rotation, Vec3 pivot, Vec3 pivotRotation) {
	setMatricesQuaternion(Vec3(0, 0, 0), rotation, Vec3(1, 1, 1), pivot, pivotRotation);
}

void Transform::initializeScalePivot(Vec3 scale, Vec3 pivot, Vec3 pivotRotation) {
	setMatrices(Vec3(0, 0, 0), Vec3(0, 0, 0), scale, pivot, pivotRotation);
}

void Transform::setTranslation(Vec3 translation) {
	translationMatrix = Mat4().initTranslation(translation);
	calculateTransformation();
}

void Transform::setRotation(Vec3 rotation) {
	rotationMatrix = Mat4().initRotation(rotation);
	calculateTransformation();
}

void Transform::setRotation(Quaternion rotation) {
	//rotationMatrix = Mat4().initRotation(rotation);
	calculateTransformation();
}

void Transform::setScale(Vec3 scale) {
	scaleMatrix = Mat4().initScale(scale);
	calculateTransformation();
}

void Transform::setPivot(Vec3 pivot, Vec3 pivotRotation) {
	pivotRotationMatrix = Mat4().initRotation(pivotRotation);
	calculateTransformation();
}

void Transform::setPivotPoint(Vec3 pivot) {
	pivotTranslationMatrix = Mat4().initTranslation(pivot);
	calculateTransformation();
}

void Transform::setPivotRotation(Vec3 pivotRotation) {
	pivotRotationMatrix = Mat4().initRotation(pivotRotation);
	calculateTransformation();
}

void Transform::calculateTransformation() {
	transformation = translationMatrix.mul(rotationMatrix.mul(scaleMatrix));
}

Vec3 Transform::getTranslation() {
	Vec3 translation = translationMatrix.getTranslationComponent();
	return translation;
}

Vec3 Transform::getRotation() {
	Vec3 rotation = rotationMatrix.getRotationComponent();
	return rotation;
}

Vec3 Transform::getScale() {
	Vec3 scale = scaleMatrix.getScaleComponent();
	return scale;
}

void Transform::setMatrices(Vec3 translation, Vec3 rotation, Vec3 scale, Vec3 pivot, Vec3 pivotRotation) {
	translationMatrix = Mat4().initTranslation(translation);
	rotationMatrix = Mat4().initRotation(rotation);
	scaleMatrix = Mat4().initScale(scale);
	pivotTranslationMatrix = Mat4().initTranslation(pivot);
	pivotRotationMatrix = Mat4().initRotation(pivotRotation);
	calculateTransformation();
}

void Transform::setMatricesQuaternion(Vec3 translation, Quaternion rotation, Vec3 scale, Vec3 pivot, Vec3 pivotRotation) {
	translationMatrix = Mat4().initTranslation(translation);
	// TODO: Add support for rotation matrices based on quaternions
	//rotationMatrix = Mat4().initRotation(rotation);
	scaleMatrix = Mat4().initScale(scale);
	pivotTranslationMatrix = Mat4().initTranslation(pivot);
	pivotRotationMatrix = Mat4().initRotation(pivotRotation);
	calculateTransformation();
}

Transform::~Transform() {

}
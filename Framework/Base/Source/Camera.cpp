/*******************************************************************************/
/*!
\file Camera.cpp
\author Lim Ngian Xin Terry
\par email: lnxterry\@gmail.com

\brief
CPP file for Camera.
*/
/*******************************************************************************/

#include "Camera.h"
#include "Mtx44.h"
#include <iostream>

using std::cout;
using std::endl;

//Constructor(s) & Destructor

Camera::Camera(const string& name) : EntityBase(name) {

	this->position.SetZero();
	this->left.Set(1, 0, 0);
	this->up.Set(0, 1, 0);
	this->target.Set(0, 0, 1);	

	isOrtho = false;
	this->nearClippingPlane = 0.3f;
	this->farClippingPlane = 1000.0f;
	SetFOV(60.0f);
	this->orthoSize = 5.0f;

}

Camera::~Camera() {
}

//FOV

void Camera::SetFOV(const float& FOV) {

	if (FOV < 1.0f) {
		this->FOV = 1.0f;
	} else if (FOV > 179.0f) {
		this->FOV = 179.0f;
	}

	this->FOV = FOV;

}

float Camera::GetFOV() {

	return this->FOV;

}

//Ortho size
void Camera::SetOrthoSize(const float& orthoSize) {

	this->orthoSize = orthoSize;

}

float Camera::GetOrthoSize() const {

	return this->orthoSize;

}

float Camera::GetOrthoWidth() const {

	return static_cast<float>(aspectRatio.x) / static_cast<float>(aspectRatio.y) * orthoSize;

}

//Projection
void Camera::SetOrtho() {

	if (this->isOrtho == false) {
		this->isOrtho = true;
	}

}

void Camera::SetPerspective() {

	if (this->isOrtho == true) {
		this->isOrtho = false;
		if (this->nearClippingPlane < 0.01f) {
			this->nearClippingPlane = 0.01f;
		}
		if (this->farClippingPlane < this->nearClippingPlane) {
			this->farClippingPlane = this->nearClippingPlane + 0.01f;
		}
	}

}

bool Camera::IsOrtho() const {

	return this->isOrtho;

}

//Clipping Planes

void Camera::SetNearClippingPlane(const float& nearClippingPlane) {
	
	if (nearClippingPlane > this->farClippingPlane) {
		this->nearClippingPlane = this->farClippingPlane - 0.01f;
	} else if (nearClippingPlane < 0.01f && isOrtho == false) {
		this->nearClippingPlane = 0.01f;
	} else {
		this->nearClippingPlane = nearClippingPlane;
	}

}

void Camera::SetFarClippingPlane(const float& farClippingPlane) {

	if (farClippingPlane < this->nearClippingPlane) {
		this->farClippingPlane = this->nearClippingPlane + 0.01f;
	} else {
		this->farClippingPlane = farClippingPlane;
	}

}

float Camera::GetNearClippingPlane() const {

	return this->nearClippingPlane;

}

float Camera::GetFarClippingPlane() const {

	return this->farClippingPlane;

}

void Camera::LookAt(Vector3 target, Vector3 up) {

	Vector3 view = target - position;
	Vector3 left = up.Cross(view);
	up = view.Cross(left);
	try {
		view.Normalize();
		left.Normalize();
		up.Normalize();
	} catch (DivideByZero) {
		cout << "LookAt(Vector3 target, Vector3 up) Error" << endl;
		return;
	}
	
	if (view.IsZero() || left.IsZero() || up.IsZero()) {
		cout << "LookAt(Vector3 target, Vector3 up) Error" << endl;
		return;
	}

	//First we need to rotate on the Y-Axis until our View aligns with the YZ-Plane.
	//To do that we need our projection on the XZ-Plane, which is the View vector without it's Y-Value.
	float y = Math::RadianToDegree(atan2(view.x, view.z));
	//Then, we need to find our how much we rotated on the X-Axis.
	float x = -Math::RadianToDegree(asin(view.y));
	//To find our Z-Rotation, we first need to know what our left vector would be if we had a Z-Rotation of 0,
	//The expected left vector if Z-Rotation is 0:
	Vector3 expectedLeft(view.z, 0, -view.x);
	//The expected up vector id Z-Rotation is 0:
	Vector3 expectedUp = view.Cross(expectedLeft);
	float z = 0.0f;
	try {
		z = Math::RadianToDegree(atan2(left.Dot(expectedUp.Normalize()), left.Dot(expectedLeft.Normalize())));
	} catch (DivideByZero) {
		cout << "LookAt(Vector3 target, Vector3 up) Error" << endl;
		return;
	}

	rotation.Set(x, y, z);
	
}

void Camera::SetPosition(float x, float y, float z) {

	this->position.Set(x, y, z);

}

void Camera::SetRotation(float x, float y, float z) {

	this->rotation.Set(x, y, z);

}

const Vector3& Camera::GetPosition() const {
	
	return this->position;

}

const Vector3& Camera::GetTarget() const {

	return this->target;

}

const Vector3& Camera::GetUp() const {

	return this->up;

}

const Vector3& Camera::GetLeft() const {

	return this->left;

}

const Vector3& Camera::GetRotation() const {

	return this->rotation;

}

void Camera::CalculateVectors() {

	enum AXIS {
		AXIS_X,
		AXIS_Y,
		AXIS_Z,
		NUM_AXIS,
	};

	Mtx44 rotationMatrix[NUM_AXIS];
	rotationMatrix[AXIS_X].SetToRotation(rotation.x, 1, 0, 0);
	rotationMatrix[AXIS_Y].SetToRotation(rotation.y, 0, 1, 0);
	rotationMatrix[AXIS_Z].SetToRotation(rotation.z, 0, 0, 1);

	Mtx44 vectorMatrix;

	vectorMatrix.SetToTranslation(1, 0, 0);
	vectorMatrix = rotationMatrix[AXIS_Y] * rotationMatrix[AXIS_X] * rotationMatrix[AXIS_Z] * vectorMatrix;
	left.Set(vectorMatrix.a[12], vectorMatrix.a[13], vectorMatrix.a[14]);

	vectorMatrix.SetToTranslation(0, 1, 0);
	vectorMatrix = rotationMatrix[AXIS_Y] * rotationMatrix[AXIS_X] * rotationMatrix[AXIS_Z] * vectorMatrix;
	up.Set(vectorMatrix.a[12], vectorMatrix.a[13], vectorMatrix.a[14]);

	vectorMatrix.SetToTranslation(0, 0, 1);
	vectorMatrix = rotationMatrix[AXIS_Y] * rotationMatrix[AXIS_X] * rotationMatrix[AXIS_Z] * vectorMatrix;
	target.Set(vectorMatrix.a[12] + position.x, vectorMatrix.a[13] + position.y, vectorMatrix.a[14] + position.z);
	
}

void Camera::Update(const double& deltaTime) {

	CalculateVectors();

}
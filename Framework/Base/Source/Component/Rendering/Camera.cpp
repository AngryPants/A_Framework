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
#include "../ComponentManager.h"

using std::cout;
using std::endl;

//Constructor(s) & Destructor

Camera::Camera(GameObject& _gameObject, PassKey<ComponentManager> _key) : Component("Camera", _gameObject, _key) {
	depth = 0.0f;
	isOrtho = false;
	this->nearClippingPlane = 0.3f;
	this->farClippingPlane = 1000.0f;
	SetFOV(60.0f);
	this->orthoSize = 5.0f;

	lodRange[LODMeshHolder::DETAIL_LEVEL::LOW_DETAILS].Set(0.0f, 0.2f);
	lodRange[LODMeshHolder::DETAIL_LEVEL::MID_DETAILS].Set(0.2f, 0.5f);
	lodRange[LODMeshHolder::DETAIL_LEVEL::HIGH_DETAILS].Set(0.5f, 1.0f);
}

Camera::~Camera() {
}

//FOV

void Camera::SetFOV(const float& _FOV) {

	if (_FOV < 1.0f) {
		this->FOV = 1.0f;
	} else if (_FOV > 179.0f) {
		this->FOV = 179.0f;
	}

	this->FOV = _FOV;

}

float Camera::GetFOV() const {

	return this->FOV;

}

//Ortho size
void Camera::SetOrthoSize(const float& _orthoSize) {

	this->orthoSize = _orthoSize;

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

void Camera::SetNearClippingPlane(const float& _nearClippingPlane) {
	
	if (_nearClippingPlane > this->farClippingPlane) {
		this->nearClippingPlane = this->farClippingPlane - 0.01f;
	} else if (_nearClippingPlane < 0.01f && isOrtho == false) {
		this->nearClippingPlane = 0.01f;
	} else {
		this->nearClippingPlane = _nearClippingPlane;
	}

}

void Camera::SetFarClippingPlane(const float& _farClippingPlane) {

	if (_farClippingPlane < this->nearClippingPlane) {
		this->farClippingPlane = this->nearClippingPlane + 0.01f;
	} else {
		this->farClippingPlane = _farClippingPlane;
	}

}

float Camera::GetNearClippingPlane() const {

	return this->nearClippingPlane;

}

float Camera::GetFarClippingPlane() const {

	return this->farClippingPlane;

}
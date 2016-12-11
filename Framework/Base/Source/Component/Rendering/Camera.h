#ifndef CAMERA_H
#define CAMERA_H

#include "Vector3.h"
#include "MyMath.h"
#include "../Component.h"
#include "../../Component/Rendering/LODMeshHolder.h"

struct AspectRatio {

public:
	//Variable(s)
	unsigned int x, y;

	//Constructor(s) & Destructor
	AspectRatio(const unsigned int& x = 16, const unsigned int& y = 9) {
		Set(x, y);
	}
	virtual ~AspectRatio() {}

	//Setter(s)
	void Set(const unsigned int& x, const unsigned int& y) {
		if (x < 1) {
			this->x = 1;
		} else {
			this->x = x;
		}
		if (y < 1) {
			this->y = 1;
		} else {
			this->y = y;
		}
		unsigned int hcf = Math::HCF(this->x, this->y);
		this->x /= hcf;
		this->y /= hcf;
	}

	float GetRatio() const {
		return static_cast<float>(x) / static_cast<float>(y);
	}

};

struct LODRange {
private:
	//Variable(s)
	float lowerBounds, upperBounds;

public:
	//Constructor(s) & Destructor
	LODRange() {
		Set(0.0f, 1.0f);
	}
	LODRange(float _lowerBounds, float _upperBounds) {
		Set(_lowerBounds, _upperBounds);
	}
	LODRange(const LODRange& _rhs) {
		Set(_rhs.lowerBounds, _rhs.upperBounds);
	}
	virtual ~LODRange() {}

	void Set(float _lowerBounds, float _upperBounds) {
		this->lowerBounds = _lowerBounds;
		this->upperBounds = _upperBounds;
	}
	float GetLowerBounds() const {
		return lowerBounds;
	}
	float GetUpperBounds() const {
		return upperBounds;
	}

};

class Camera : public Component {

protected:
	//Perspective
	float FOV;
	float nearClippingPlane;
	float farClippingPlane;
	
	//Orthographic
	bool isOrtho;
	
public:	
	float depth;
	float orthoSize;
	AspectRatio aspectRatio;
	LODRange lodRange[LODMeshHolder::DETAIL_LEVEL::NUM_DETAIL_LEVEL]; //The lower bounds of LOW_DETAIL and upper bounds of HIGH_DETAIL are ignored.

	//Constructor(s) & Destructor
	Camera(GameObject& gameObject);
	virtual ~Camera();

	void SetFOV(const float& FOV);
	float GetFOV() const;

	void SetOrthoSize(const float& orthoSize);
	float GetOrthoSize() const;
	float GetOrthoWidth() const;

	void SetOrtho();
	void SetPerspective();
	bool IsOrtho() const;

	void SetNearClippingPlane(const float& nearClippingPlane);
	void SetFarClippingPlane(const float& nearClippingPlane);
	float GetNearClippingPlane() const;
	float GetFarClippingPlane() const;
	void SetPosition(float x = 0.0f, float y = 0.0f, float z = 0.0f);
	void LookAt(Vector3 target, Vector3 up = Vector3(0, 1, 0));
	void SetRotation(float x = 0.0f, float y = 0.0f, float z = 0.0f);
	
};

#endif
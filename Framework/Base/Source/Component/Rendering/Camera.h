#ifndef CAMERA_H
#define CAMERA_H

#include "Vector3.h"
#include "MyMath.h"
#include "../Component.h"

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
	
	//Constructor(s) & Destructor
	Camera(GameObject& gameObject);
	virtual ~Camera();

	void SetFOV(const float& FOV);
	float GetFOV();

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
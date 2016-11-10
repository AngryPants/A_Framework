#include "SphereCollider.h"
#include "MyMath.h"

//Constructor(s) & Destructor
SphereCollider::SphereCollider(GameObject& gameObject) : Collider("Sphere Collider", gameObject) {
	radius = 0.5f;
}

SphereCollider::~SphereCollider() {
}

//Interface Function(s)
void SphereCollider::SetRadius(const float radius) {
	if (radius > Math::EPSILON) {
		this->radius = radius;
	} else {
		this->radius = Math::EPSILON;
	}
}

float SphereCollider::GetRadius() const {
	return radius;
}
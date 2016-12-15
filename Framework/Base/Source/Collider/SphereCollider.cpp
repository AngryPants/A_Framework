#include "SphereCollider.h"

//Constructor(s) & Destructor
SphereCollider::SphereCollider(GameObjectID _gameObjectID) : Collider("Sphere Collider", _gameObjectID) {
	radius = 0.5f;
}

SphereCollider::SphereCollider(const SphereCollider& _rhs) : Collider(_rhs) {
	this->radius = _rhs.radius;
}

SphereCollider::~SphereCollider() {
}

//Function(s)
void SphereCollider::SetRadius(const float _radius) {
	this->radius = Math::Max(_radius, 0.0f);
}

float SphereCollider::GetRadius() const {
	return this->radius;
}

//Virtual Function(s)
SphereCollider* SphereCollider::Copy() {
	return new SphereCollider(*this);
}
#include "ContactPoint.h"

//Constructor(s) & Destructor
ContactPoint::ContactPoint(const Collider* _thisCollider, const Collider* _otherCollider)
: thisCollider(_thisCollider), otherCollider(_otherCollider) {
	separation = -1.0f;
}

ContactPoint::~ContactPoint() {
}
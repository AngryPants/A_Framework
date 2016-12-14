#ifndef CONTACT_POINT_H
#define CONTACT_POINT_H

#include "Vector3.h"

class Collider;

struct ContactPoint {
public:
	//Variable(s)
	const Collider* otherCollider;
	const Collider* thisCollider;
	Vector3 normal;
	Vector3 point;
	float separation;

	//Constructor(s) & Destructor
	ContactPoint(const Collider* _thisCollider, const Collider* _otherCollider);
	virtual ~ContactPoint();

};

#endif
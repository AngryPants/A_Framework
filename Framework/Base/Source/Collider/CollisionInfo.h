#ifndef COLLISION_INFO_H
#define COLLISION_INFO_H

#include "Vector3.h"
#include "ContactPoint.h"
#include "../Others/IDGenerator.h"
#include <vector>

using std::vector;

struct CollisionInfo {
public:
	//Variable(s)
	GameObjectID gameObject; //The GameObject we are colliding with.
	vector<ContactPoint> contacts;
	Vector3 relativeVelocity;

	//Constructor(s) & Destructor
	CollisionInfo() {}
	virtual ~CollisionInfo() {}

};

#endif
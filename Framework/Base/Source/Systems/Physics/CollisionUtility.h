#ifndef COLLISION_UTILITY_H
#define COLLISION_UTILITY_H

#include "Vector3.h"

class CollisionUtility {
private:
	//Constructor(s) & Destructor
	CollisionUtility();
	virtual ~CollisionUtility();

public:
	//Interface Function(s)
	/*
		Return:
		Vector3 collisionPoint (If Applicable)
		Vector3 collisionNormal (If Applicable)
		double collisionTime
	*/

	void CheckCollisionLineSphere(Vector3 positionA, float radiusA, Vector3 positionB, float radiusB);
	void CheckCollisionLineAABB(Vector3 positionA, float radiusA, Vector3 positionB, float radiusB);
	void CheckCollisionSphereSphere(Vector3 positionA, float radiusA, Vector3 positionB, float radiusB);
	void CheckCollisionSphereAABB(Vector3 positionSphere, float radiusSphere, Vector3 positionBox, Vector3 sizeBox);
	void CheckCollisionAABB(Vector3 positionA, Vector3 sizeA, Vector3 positionB, Vector3 sizeB);

};

#endif
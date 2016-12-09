#ifndef COLLISION_UTILITY_H
#define COLLISION_UTILITY_H

#include "Vector3.h"
//Generic Collision Utility, returns whether 2 objects when pass each other or not
class CollisionUtility {
private:
	//Constructor(s) & Destructor
	CollisionUtility();
	virtual ~CollisionUtility();

	//private inline functions
	bool inline GetIntersection(float fDst1, float fDst2, Vector3 P1, Vector3 P2, Vector3 &Hit);
	bool inline InBox(Vector3 Hit, Vector3 B1, Vector3 B2, const int Axis);

public:
	//Interface Function(s)
	/*
		Return:
		Vector3 collisionPoint (If Applicable)
		Vector3 collisionNormal (If Applicable)
		double collisionTime
	*/
	//Accurate Collision Sphere
	bool CheckAccurateSphereSphereCollision(Vector3 positionA, Vector3& velocityA, float radiusA, Vector3 positionB, Vector3 velocityB, float radiusB);
	
	//RayCasting (infiniteLength)
	bool CheckCollisionRaySphere(Vector3 lineOrigin, Vector3 dirVec, Vector3 positionSphere, float sphereRadius, Vector3& collisionPoint, Vector3& collisionNormal);
	bool CheckCollisionRayBox(Vector3 lineOrigin, Vector3 lineForward, Vector3 positionBox, Vector3 sizeBox, float& distance);
	bool CheckCollisionRayPlane(const Vector3& lineOrigin, const Vector3& dirVec, const Vector3& planeCenter, const Vector3& planeNormal, float& timeTaken);
	bool CheckCollisionRayDisk(const Vector3 &normal, const Vector3 &planeCenter, const float &radius, const Vector3 &lineOrigin, const Vector3 &lineDir);

	//Line Collision (finite length)
	bool CheckCollisionLineSphere(Vector3 lineOrigin, Vector3 lineForward, float lineLength, Vector3 position, float radius);
	bool CheckCollisionLineAABB(Vector3 lineOrigin, Vector3 lineForward, float lineLength, Vector3 positionBox, Vector3 sizeBox, Vector3& hit);
	bool CheckCollisionLineBox(Vector3 minPosBox, Vector3 maxPosBox, Vector3 lineOrigin, Vector3 lineEnd, Vector3 &Hit);
	
	//Sphere collision
	bool CheckCollisionSpherePoint(Vector3 positionA, float radiusA, Vector3 positionB);
	bool CheckCollisionSphereSphere(Vector3 positionA, float radiusA, Vector3 positionB, float radiusB);
	bool CheckCollisionSphereAABB(Vector3 positionSphere, float radiusSphere, Vector3 positionBox, Vector3 sizeBox);
	bool CheckCollisionSphereAABBMinMax(Vector3 positionSphere, float radiusSphere, Vector3 minPosBox, Vector3 maxPosBox);

	//AABB collision Standard Box
	bool CheckCollisionAABB(Vector3 positionA, Vector3 sizeA, Vector3 positionB, Vector3 sizeB);
	//Generic AABB
	bool CheckCollisionAABBMinMax(Vector3 minPosA, Vector3 maxPosA, Vector3 minPosB, Vector3 maxPosB);

	//AABB with Point
	bool CheckCollisionAABBPoint(Vector3 positionA, Vector3 sizeA,Vector3 positionB);
	bool CheckCollisionAABBPointMinMax(Vector3 minPosA, Vector3 maxPosA, Vector3 positionB);
};

#endif
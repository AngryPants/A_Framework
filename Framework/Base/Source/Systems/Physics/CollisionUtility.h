#ifndef COLLISION_UTILITY_H
#define COLLISION_UTILITY_H

#include "Vector3.h"
//Generic Collision Utility, returns whether 2 objects when pass each other or not
class CollisionUtility {
private:
	//Constructor(s) & Destructor
	CollisionUtility();
	virtual ~CollisionUtility();

	//Private inline Functions
	static bool inline GetIntersection(float fDst1, float fDst2, Vector3 P1, Vector3 P2, Vector3 &Hit);
	static bool inline InBox(Vector3 Hit, Vector3 B1, Vector3 B2, const int Axis);

public:
	//Interface Function(s)
	//Accurate Collision Sphere
	static bool CheckAccurateSphereSphereCollision(Vector3 positionA, Vector3& velocityA, float radiusA, Vector3 positionB, Vector3 velocityB, float radiusB);
	static bool CheckCollisionSphereSphere_Mathematical(const Vector3& _positionA, const Vector3& _velocityA, float _radiusA,
														const Vector3& _positionB, const Vector3& _velocityB, float _radiusB,
														double deltaTime,
														Vector3& _collisionPoint, Vector3& _collisionNormal, float& _separation);

	//RayCasting (infiniteLength)
	static bool CheckCollisionRaySphere(Vector3 lineOrigin, Vector3 dirVec, Vector3 positionSphere, float sphereRadius, Vector3& collisionPoint, Vector3& collisionNormal);
	static bool CheckCollisionRayBox(Vector3 lineOrigin, Vector3 lineForward, Vector3 positionBox, Vector3 sizeBox, float& distance);
	static bool CheckCollisionRayPlane(const Vector3& lineOrigin, const Vector3& dirVec, const Vector3& planeCenter, const Vector3& planeNormal, float& timeTaken);
	static bool CheckCollisionRayDisk(const Vector3 &normal, const Vector3 &planeCenter, const float &radius, const Vector3 &lineOrigin, const Vector3 &lineDir);

	//Line Collision (finite length)
	static bool CheckCollisionLineSphere(Vector3 lineOrigin, Vector3 lineForward, float lineLength, Vector3 position, float radius);
	static bool CheckCollisionLineAABB(Vector3 lineOrigin, Vector3 lineForward, float lineLength, Vector3 positionBox, Vector3 sizeBox, Vector3& hit);
	static bool CheckCollisionLineBox(Vector3 minPosBox, Vector3 maxPosBox, Vector3 lineOrigin, Vector3 lineEnd, Vector3 &Hit);
	
	//Sphere collision
	static bool CheckCollisionSpherePoint(Vector3 positionA, float radiusA, Vector3 positionB);
	static bool CheckCollisionSphereSphere_Empirical(Vector3 positionA, float radiusA, Vector3 positionB, float radiusB);
	static bool CheckCollisionSphereAABB(Vector3 positionSphere, float radiusSphere, Vector3 positionBox, Vector3 sizeBox);
	static bool CheckCollisionSphereAABBMinMax(Vector3 positionSphere, float radiusSphere, Vector3 minPosBox, Vector3 maxPosBox);

	//AABB collision Standard Box
	static bool CheckCollisionAABB(Vector3 positionA, Vector3 sizeA, Vector3 positionB, Vector3 sizeB);

	//Generic AABB
	static bool CheckCollisionAABBMinMax(Vector3 minPosA, Vector3 maxPosA, Vector3 minPosB, Vector3 maxPosB);

	//AABB with Point
	static bool CheckCollisionAABBPoint(Vector3 positionA, Vector3 sizeA,Vector3 positionB);
	static bool CheckCollisionAABBPointMinMax(Vector3 minPosA, Vector3 maxPosA, Vector3 positionB);
};

#endif
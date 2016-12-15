#ifndef COLLISION_SYSTEM_H
#define COLLISION_SYSTEM_H

#include "SingletonTemplate.h"
#include "Vector3.h"
#include <string>

//Forward Declaration
class Grid;
class GameObject;
class Rigidbody;
class SphereCollider;
class AABBCollider;

using std::string;

class PhysicsSystem : public Singleton<PhysicsSystem> {

	friend class Singleton<PhysicsSystem>;

private:
	//Variable(s)
	double deltaTime;

	//Constructor(s) & Destructor
	PhysicsSystem();
	virtual ~PhysicsSystem();

	//Private Function(s)	
	void CheckGrid(Grid& _grid);
	void CheckGrid(Grid& _grid, Grid& _grid2);
	void SortCollisionType(GameObject* _a, GameObject* _b);

	void SortCollisionSphere(GameObject* _a, GameObject* _b);
	void SortCollisionAABB(GameObject* _a, GameObject* _b);

	//void Collision_AABB_AABB(GameObject* _a, GameObject* _b);
	//void Collision_AABB_Sphere(GameObject* _a, GameObject* _b);

	//Sphere-Sphere
	void Collision_Sphere_Sphere(GameObject* _a, GameObject* _b);
	void Response_Sphere_Sphere(SphereCollider* _colliderA, Rigidbody* _rigidbodyA, const Vector3& positionA, SphereCollider* _colliderB, Rigidbody* _rigidbodyB, const Vector3& _positionB);
	void Response_Sphere_Sphere(SphereCollider* _colliderA, Rigidbody* _rigidbodyA, const Vector3& positionA, SphereCollider* _colliderB, GameObject* _goB, const Vector3& _positionB);

	//AABB-AABB
	void Collision_AABB_AABB(GameObject* _a, GameObject* _b);
	void Response_AABB_AABB(AABBCollider* _colliderA, Rigidbody* _rigidbodyA, const Vector3& _positionA, AABBCollider* _colliderB, Rigidbody* _rigidbodyB, const Vector3& _positionB);
	void Response_AABB_AABB(AABBCollider* _colliderA, Rigidbody* _rigidbodyA, const Vector3& _positionA, AABBCollider* _colliderB, GameObject* _goB, const Vector3& _positionB);

	//Sphere-AABB
	void Collision_Sphere_AABB(GameObject* _a, GameObject* _b);

	void UpdateGravity(const string& _space);	
	void UpdateCollision(const string& _space);
	void UpdateMovement(const string& _space);

public:
	//Function(s)
	void UpdateDeltaTime(const string& _space, double _deltaTime);
	void Update(const string& _space);	

};

#endif
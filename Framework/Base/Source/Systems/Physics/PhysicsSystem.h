#ifndef COLLISION_SYSTEM_H
#define COLLISION_SYSTEM_H

#include "SingletonTemplate.h"
#include <string>

//Forward Declaration
class Grid;
class GameObject;

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
	//void Collision_AABB_AABB(GameObject* _a, GameObject* _b);
	//void Collision_AABB_Sphere(GameObject* _a, GameObject* _b);

	void Collision_Sphere_Sphere(GameObject* _a, GameObject* _b);

public:
	//Function(s)
	void UpdateDeltaTime(const string& _space, double _deltaTime);
	void UpdateGravity(const string& _space);	
	void UpdateCollision(const string& _space);
	void UpdateMovement(const string& _space);

};

#endif
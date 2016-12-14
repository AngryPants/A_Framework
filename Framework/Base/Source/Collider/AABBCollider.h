#ifndef AABB_COLLIDER_H
#define AABB_COLLIDER_H

#include "Collider.h"

class AABBCollider : public Collider {
	
public:
	//Variable(s)
	Vector3 size;

	//Constructor(s) & Destructor
	AABBCollider(GameObjectID _gameObjectID);
	AABBCollider(const AABBCollider& _rhs);
	virtual ~AABBCollider();

	//Virtual Function(s)
	virtual AABBCollider* Copy();

};

#endif
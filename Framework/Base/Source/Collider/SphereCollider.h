#ifndef SPHERE_COLLIDER_H
#define SPHERE_COLLIDER_H

#include "Collider.h"

class SphereCollider : public Collider {

private:
	//Variable(s)
	float radius;

public:
	//Constructor(s) & Destructor
	SphereCollider(GameObjectID _gameObjectID);
	SphereCollider(const SphereCollider& _rhs);
	virtual ~SphereCollider();
	
	//Function(s)
	void SetRadius(const float _radius);
	float GetRadius() const;

	//Virtual Function(s)
	virtual SphereCollider* Copy();

};

#endif
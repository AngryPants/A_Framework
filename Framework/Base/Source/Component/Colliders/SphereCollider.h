#ifndef SPHERE_COLLIDER_H
#define SPHERE_COLLIDER_H

#include "Collider.h"
class SphereCollider : public Collider
{
public:
	//public variable(s)
	float radius;
	//Constructors & Destructors	
	SphereCollider(GameObject &gameObject);
	virtual ~SphereCollider();

};




#endif
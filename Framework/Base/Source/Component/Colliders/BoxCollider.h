#ifndef BOX_COLLIDER_H
#define BOX_COLLIDER_H

#include "Collider.h"

class BoxCollider : public Collider
{
public:
	//public Variable(s)
	Vector3 boxMin, boxMax;
	//Constructors & Destructors	
	BoxCollider(GameObject &gameObject);
	virtual ~BoxCollider();

};



#endif
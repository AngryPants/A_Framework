#ifndef SPHERE_TRIGGER_H
#define SPHERE_TRIGGER_H

#include "../Component.h"
#include "Vector3.h"
//Triggers are supposed to be Simple and Cheap Contact Checks , MultiPurpose Not only for Collision
class SphereTrigger : public Component
{
private:
	float radius;

public:
	//Constructors & Destructors	
	SphereTrigger(GameObject &gameObject);
	virtual ~SphereTrigger();
	//Public Functions
	void SetRadius(float newRadius);

	float GetRadius();
	//Public Variable(s)
	bool activeTrigger;
};






#endif
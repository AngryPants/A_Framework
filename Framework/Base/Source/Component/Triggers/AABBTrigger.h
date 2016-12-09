#ifndef AABB_TRIGGER_H
#define AABB_TRIGGER_H

#include "../Component.h"
#include "Vector3.h"

//Triggers are supposed to be Simple and Cheap Contact Checks , MultiPurpose Not only for Collision
class AABBTrigger : public Component
{
private:
	Vector3 AABBMin, AABBMax;

public:
	//Constructors & Destructors	
	AABBTrigger(GameObject &gameObject);
	virtual ~AABBTrigger();

	//Public Functions
	void SetAABB(Vector3 AABBMin,Vector3 AABBMax);
	void SetAABB(Vector3 size);
	void SetAABB(float x, float y, float z);

	void GetAABB(Vector3& AABBMin, Vector3& AABBMax);
	Vector3 GetMinAABB();
	Vector3 GetMaxAABB();

	//Public Variable(s)
	bool activeTrigger;
};









#endif
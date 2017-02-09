#ifndef CHASE_SCRIPT_H
#define CHASE_SCRIPT_H

#include "../Script/Script.h"
#include "../GameObject/GameObject.h"

class AIChaseScript : public Script
{
public:
	Vector3 playerPosition;
	bool hasPlayer;
	//constructor and destructor
	AIChaseScript(GameObject& gameObject);
	virtual ~AIChaseScript();

	//Function(s)
	virtual void OnTriggerStay(const Collider& _collider);
	virtual void Update(double deltaTime);
};

#endif
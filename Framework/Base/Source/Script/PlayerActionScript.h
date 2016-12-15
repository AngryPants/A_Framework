#ifndef PLAYER_ACTION_SCRIPT
#define PLAYER_ACTION_SCRIPT
			   
#include "Script.h"

class PlayerActionScript : public Script
{
public:
	PlayerActionScript(GameObject& gameObject);
	virtual ~PlayerActionScript();
	
	virtual void OnTriggerStay(const Collider& _collider);
	virtual void Update(double deltaTime){}
};

#endif
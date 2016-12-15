#ifndef PLAYER_PICKUP_ACTION_SCRIPT
#define PLAYER_PICKUP_ACTION_SCRIPT
			   
#include "Script.h"

class PlayerPickUpActionScript : public Script
{
public:
	PlayerPickUpActionScript(GameObject& gameObject);
	virtual ~PlayerPickUpActionScript();
	
	virtual void OnTriggerStay(const Collider& _collider);
	virtual void Update(double deltaTime){}
};

#endif
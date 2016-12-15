#ifndef AI_SHOOTING_SCRIPT_H
#define AI_SHOOTING_SCRIPT_H

#include "../Script/ShootingScript.h"

class AIShootingScript : public ShootingScript
{
public:
	AIShootingScript(GameObject& gameObject);
	virtual ~AIShootingScript();

	virtual void OnTriggerStay(const Collider& _collider);

	virtual void ShootBullet();

	virtual bool ExtraCondition();
};


#endif
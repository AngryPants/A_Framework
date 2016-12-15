#ifndef AI_SHOOTING_SCRIPT_H
#define AI_SHOOTING_SCRIPT_H

#include "../Script/ShootingScript.h"

class AIShootingScript : public ShootingScript
{

public:
	bool toShoot;
	Vector3 playerPosition;
	AIShootingScript(GameObject& gameObject);
	virtual ~AIShootingScript();

	virtual void OnTriggerStay(const Collider& _collider);

	virtual void Update(double deltaTime);

	virtual void ShootBullet();

	virtual bool ExtraCondition();
};


#endif
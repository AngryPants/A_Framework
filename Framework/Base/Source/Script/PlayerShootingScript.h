#ifndef PLAYER_SHOOTING_SCRIPT_H
#define PLAYER_SHOOTING_SCRIPT_H

#include "../Script/ShootingScript.h"
#include "../Input/InputManager.h"

class PlayerShootingScript : public ShootingScript
{
public:

	bool toShoot,toReload;
	PlayerShootingScript(GameObject& gameObject);
	virtual ~PlayerShootingScript();

	virtual void ShootBullet();
	virtual void Update(double deltaTime);
	virtual void OnTriggerStay(const Collider& _collider);
	virtual bool ManualReload();
	virtual bool ExtraCondition();
};


#endif
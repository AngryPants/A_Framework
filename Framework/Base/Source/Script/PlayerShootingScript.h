#ifndef PLAYER_SHOOTING_SCRIPT_H
#define PLAYER_SHOOTING_SCRIPT_H

#include "../Script/ShootingScript.h"
#include "../Input/InputManager.h"

class PlayerShootingScript : public ShootingScript
{
public:
	PlayerShootingScript(GameObject& gameObject);
	virtual ~PlayerShootingScript();

	virtual void ShootBullet();

	virtual bool ManualReload();
	virtual bool ExtraCondition();
};


#endif
#ifndef PLAYER_SHOOTING_SCRIPT_H
#define PLAYER_SHOOTING_SCRIPT_H

#include "../Script/ShootingScript.h"
#include "../Component/Colliders/SphereCollider.h"
#include "../Input/InputManager.h"

class PlayerShootingScript : public ShootingScript
{
public:

	PlayerShootingScript(GameObject& gameObject);
	virtual ~PlayerShootingScript();

	virtual void ShootBullet(const string& bulletName = "bullet", const string&filepath = "OBJ//Default//Sphere.obj", const float& lifeTime = 5.f, const float& bulletSpeed = 20.f, const float& bulletSize = 10.f);

	virtual bool ExtraCondition();
};








#endif
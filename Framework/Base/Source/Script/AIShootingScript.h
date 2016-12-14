#ifndef AI_SHOOTING_SCRIPT_H
#define AI_SHOOTING_SCRIPT_H

#include "../Script/ShootingScript.h"
#include "../Component/Colliders/SphereCollider.h"

class AIShootingScript : public ShootingScript
{
public:
	AIShootingScript(GameObject& gameObject);

	virtual ~AIShootingScript();

	virtual void ShootBullet(const string& bulletName = "bullet", const string&filepath = "", const float& lifeTime = 5.f, const float& bulletSpeed = 20.f, const float& bulletSize = 10.f);

	virtual bool ExtraCondition();
};








#endif
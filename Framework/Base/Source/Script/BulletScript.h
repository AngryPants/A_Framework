#ifndef BULLET_SCRIPT_H
#define BULLET_SCRIPT_H

#include "../Script/Script.h"
#include "../GameObject/GameObject.h"
#include "../Component/Physics/Rigidbody.h"
#include "../Component/Game/Bullet.h"

//Base Class
class BulletScript : public Script
{
public:
	BulletScript(GameObject& gameObject) :Script(gameObject)
	{
	}
	virtual ~BulletScript(){};

	//virtual void OnCollisionStay(const CollisionInfo& _collisionInfo){};
	virtual void Update(double deltaTime)
	{
		GetGameObject().GetComponent<BulletComponent>().lifeTime -= deltaTime;
		// if bullet time is up
		if (GetGameObject().GetComponent<BulletComponent>().lifeTime <= 0.f)
		{
			GetGameObject().Destroy();
		}
	}
};


#endif
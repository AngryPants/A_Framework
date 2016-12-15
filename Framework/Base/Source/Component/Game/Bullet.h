#ifndef BULLET_COMPONENT_H
#define BULLET_COMPONENT_H

#include "../Physics/RigidBody.h"
#include "../../GameObject/GameObject.h"
#include "../Rendering/MeshHolder.h"
#include "Vector3.h"

class BulletComponent : public Component
{
public:
	//variables 
	float lifeTime;
	float bulletImpulse;
	//Constructors
	BulletComponent(GameObject& gameObject, PassKey<ComponentManager> _key)
		: Component("Bullet Component", gameObject, _key)
		, lifeTime(5.f)
		, bulletImpulse(50.f) 
	{
		if (!gameObject.HasComponent<Rigidbody>())
		{
			gameObject.AddComponent<Rigidbody>();
		}
	}
	virtual ~BulletComponent(){}

};


#endif
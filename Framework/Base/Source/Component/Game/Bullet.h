#ifndef BULLET_COMPONENT_H
#define BULLET_COMPONENT_H

#include "../Physics/RigidBody.h"
#include "../Colliders/SphereCollider.h"
#include "../../GameObject/GameObject.h"
#include "../Rendering/MeshHolder.h"
#include "Vector3.h"

class BulletComponent : public Component
{
public:
	//variables 
	float lifeTime;
	float bulletSpeed;
	//Constructors
	BulletComponent(GameObject& gameObject)
		: Component("bullet", gameObject)
		, lifeTime(5.f)
		, bulletSpeed(20.f) 
	{
		if (!gameObject.HasComponent<Rigidbody>())
			gameObject.AddComponent<Rigidbody>();

		if (!gameObject.HasComponent<SphereCollider>())
			gameObject.AddComponent<SphereCollider>();

		gameObject.GetComponent<SphereCollider>().centre = gameObject.GetComponent<Transform>().GetPosition();
	}
	virtual ~BulletComponent(){}

};


#endif
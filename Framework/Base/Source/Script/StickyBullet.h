#ifndef STICKY_BULLET_H
#define STICKY_BULLET_H

#include "BulletScript.h"
#include "../GameObject/GameObjectManager.h"
#include "../Collider/SphereCollider.h"
#include "../Component/Game/Health.h"

class StickyBulletScript : public BulletScript
{
public:
	StickyBulletScript(GameObject & gameObject) : BulletScript(gameObject)
	{}
	virtual ~StickyBulletScript() {}
	
	virtual void OnCollisionStay(const CollisionInfo& _info)
	{
		GameObject* temp = GameObjectManager::GetInstance().GetGameObjectByID(_info.gameObject);
		if (temp->name == "Enemy" && GetGameObject().GetParent() == nullptr)
		{
			const SphereCollider* collider = dynamic_cast<const SphereCollider*>(_info.contacts[0].otherCollider);
			if (collider != nullptr) {
				GetGameObject().SetParent(*temp);
				if (GetGameObject().HasActiveComponent<Rigidbody>()) {
					GetGameObject().RemoveComponent<Rigidbody>();
				}

				const SphereCollider* thisCollider = dynamic_cast<const SphereCollider*>(_info.contacts[0].thisCollider);

				if (thisCollider != nullptr) {
					GetGameObject().GetComponent<Transform>().SetLocalPosition(_info.contacts[0].normal * (collider->GetRadius() + thisCollider->GetRadius()));
				}
				else {
					GetGameObject().GetComponent<Transform>().SetLocalPosition(_info.contacts[0].normal * collider->GetRadius());
				}
				
			}			
		}
	};

	virtual void Update(double deltaTime)
	{
		GetGameObject().GetComponent<BulletComponent>().lifeTime -= deltaTime;
		// if bullet time is up
		if (GetGameObject().GetComponent<BulletComponent>().lifeTime <= 0.f)
		{
			if (GetGameObject().GetParent() != nullptr && GetGameObject().GetParent()->HasActiveComponent<HealthComponent>()) {
				GetGameObject().GetParent()->GetComponent<HealthComponent>().TakeDamage(GetGameObject().GetComponent<BulletComponent>().bulletDamage);
				GetGameObject().Destroy();
			}
		}
	}

};




#endif
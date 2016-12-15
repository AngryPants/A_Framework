#ifndef PLAYER_BULLET_SCRIPT
#define PLAYER_BULLET_SCRIPT

#include "BulletScript.h"
#include "../GameObject/GameObjectManager.h"
#include "../Component/Game/Health.h"

class PlayerBulletScript : public BulletScript
{
public:
	PlayerBulletScript(GameObject& gameObject) : BulletScript(gameObject)
	{
	}
	virtual ~PlayerBulletScript(){}

	virtual void OnCollisionStay(const CollisionInfo& _collider)
	{
		GameObject* temp = GameObjectManager::GetInstance().GetGameObjectByID(_collider.gameObject);
		if (temp->name == "Enemy")
		{
			temp->GetComponent<HealthComponent>().SetHealth(temp->GetComponent<HealthComponent>().GetCurrentHealth() - GetGameObject().GetComponent<BulletComponent>().bulletDamage);
			GetGameObject().Destroy();
		}
	}

};

#endif
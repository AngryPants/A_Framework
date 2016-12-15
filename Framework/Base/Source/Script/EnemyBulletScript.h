#ifndef ENEMY_BULLET_SCRIPT
#define ENEMY_BULLET_SCRIPT

#include "BulletScript.h"
#include "../GameObject/GameObjectManager.h"
#include "../Component/Game/Health.h"

class EnemyBulletScript : public BulletScript
{
public:
	EnemyBulletScript(GameObject& gameObject) : BulletScript(gameObject)
	{
	}
	virtual ~EnemyBulletScript(){}

	virtual void OnCollisionStay(const Collider& _collider)
	{
		GameObject* temp = GameObjectManager::GetInstance().GetGameObjectByID(_collider.gameObjectID);
		if (temp->name == "Player")
		{
			temp->GetComponent<HealthComponent>().SetHealth(temp->GetComponent<HealthComponent>().GetCurrentHealth() - GetGameObject().GetComponent<BulletComponent>().bulletDamage);
			GetGameObject().Destroy();
		}
	}

};

#endif
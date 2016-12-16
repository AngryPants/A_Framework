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
		cout << temp->name << endl;
		if (temp->name == "Enemy" || temp->name == "Core")
		{
			temp->GetComponent<HealthComponent>().TakeDamage(GetGameObject().GetComponent<BulletComponent>().bulletDamage);
			cout << "Enemy Health : " << temp->GetComponent<HealthComponent>().GetCurrentHealth() << endl;
			GetGameObject().Destroy();
		}
	}

};

#endif
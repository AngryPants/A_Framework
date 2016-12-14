#include "PlayerShootingScript.h"
#include "../GameObject/GameObjectFactory.h"

PlayerShootingScript::PlayerShootingScript(GameObject& gameObject)
	: ShootingScript(gameObject)
{
}

PlayerShootingScript::~PlayerShootingScript()
{
}

void PlayerShootingScript::ShootBullet(const string& bulletName, const string&filepath, const float& lifeTime, const float& bulletSpeed, const float& bulletSize)
{
	if (filepath.size() > 0)
	{
		GameObject& temp = GameObjectFactory::CreateBulletBase(GetGameObject().GetSpace());
		temp.GetComponent<BulletComponent>().bulletSpeed = bulletSpeed;
		temp.GetComponent<BulletComponent>().lifeTime = lifeTime;
		temp.GetComponent<SphereCollider>().radius = bulletSize;
		temp.GetComponent<MeshHolder>().mesh = MeshBuilder::GetInstance().GenerateOBJ(bulletName,filepath);
	}
}

bool PlayerShootingScript::ExtraCondition()
{
	return (InputManager::GetInstance().GetInputInfo().keyDown[INPUT_PLAYERSHOOT] /*&& GetGameObject().HasComponent<RifleComponent>()*/);
}
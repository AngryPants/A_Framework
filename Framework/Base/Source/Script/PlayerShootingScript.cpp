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
		GameObjectFactory::CreateBullet(GetGameObject().GetSpace(), bulletName, filepath, lifeTime, bulletSpeed, bulletSize);
}

bool PlayerShootingScript::ExtraCondition()
{
	return (InputManager::GetInstance().GetInputInfo().keyDown[INPUT_PLAYERSHOOT] && GetGameObject().HasComponent<RifleComponent>());
}
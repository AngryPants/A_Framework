#include "AIShootingScript.h"
#include "../GameObject/GameObjectFactory.h"

AIShootingScript::AIShootingScript(GameObject& gameObject) : ShootingScript(gameObject)
{
}

AIShootingScript::~AIShootingScript()
{

}

void AIShootingScript::ShootBullet(const string& bulletName , const string&filepath, const float& lifeTime, const float& bulletSpeed, const float& bulletSize)
{
	if (filepath.size() > 0)
		GameObjectFactory::CreateBullet(GetGameObject().GetSpace(), bulletName, filepath, lifeTime, bulletSpeed, bulletSize);
}

bool AIShootingScript::ExtraCondition()
{
	return (GetGameObject().GetComponent<SphereCollider>().name == "AI_DetectionRadius" && GetGameObject().tag == "player");
	//&& GetGameObject().GetComponent<SphereCollider>().OnTriggerEnter())
}
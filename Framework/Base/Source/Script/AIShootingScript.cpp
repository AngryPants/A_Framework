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
	{
		GameObject& temp = GameObjectFactory::CreateBulletBase(GetGameObject().GetSpace());
		temp.GetComponent<BulletComponent>().bulletSpeed = bulletSpeed;
		temp.GetComponent<BulletComponent>().lifeTime = lifeTime;
		temp.GetComponent<ColliderGroup<SphereCollider>>().colliders[0].SetRadius(bulletSize);
		temp.GetComponent<MeshHolder>().mesh = MeshBuilder::GetInstance().GenerateOBJ(bulletName, filepath);
	}
}

bool AIShootingScript::ExtraCondition()
{
	return false;
}
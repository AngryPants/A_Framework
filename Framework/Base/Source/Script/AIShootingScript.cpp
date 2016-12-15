#include "AIShootingScript.h"
#include "../GameObject/GameObjectFactory.h"

AIShootingScript::AIShootingScript(GameObject& gameObject) : ShootingScript(gameObject)
{
}

AIShootingScript::~AIShootingScript()
{

}

void AIShootingScript::ShootBullet()
{
	GameObject& temp = GameObjectFactory::CreateBulletBase(GetGameObject().GetSpace());
	Vector3 forwardRotation = LookAt(GetGameObject().GetComponent<Transform>().GetForward(), GetGameObject().GetComponent<Transform>().GetUp());
	temp.GetComponent<Transform>().SetLocalPosition(GetGameObject().GetComponent<Transform>().GetPosition());
	temp.GetComponent<Transform>().SetLocalRotation(forwardRotation);
	temp.GetComponent<BulletComponent>().bulletImpulse = 50.f;
	temp.GetComponent<BulletComponent>().lifeTime = 5.f;
	temp.GetComponent<ColliderGroup<SphereCollider>>().colliders[0].SetRadius(1.f);
	temp.GetComponent<MeshHolder>().mesh = MeshBuilder::GetInstance().GenerateOBJ("Enemy Bullet", "OBJ//Default//Sphere.obj");
	temp.GetComponent<Rigidbody>().SetMass(0.01f);
	temp.GetComponent<Rigidbody>().AddRelativeForce(GetGameObject().GetComponent<Transform>().GetForward() * temp.GetComponent<BulletComponent>().bulletImpulse, FORCE_MODE::FM_IMPULSE);

}

bool AIShootingScript::ExtraCondition()
{
	Collider* collideInfo;
	OnTriggerStay(*collideInfo);
	return (GameObjectManager::GetInstance().GetGameObjectByID(collideInfo->gameObjectID)->name == "Player");
}


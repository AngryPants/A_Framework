#include "AIShootingScript.h"
#include "../GameObject/GameObjectFactory.h"
#include "../Script/EnemyBulletScript.h"


AIShootingScript::AIShootingScript(GameObject& gameObject) : ShootingScript(gameObject)
{
}

AIShootingScript::~AIShootingScript()
{

}

void AIShootingScript::OnTriggerStay(const Collider& _collider)
{
	if (GameObjectManager::GetInstance().GetGameObjectByID(_collider.gameObjectID)->name == "Player");	
}

void AIShootingScript::ShootBullet()
{
	Collider* collideInfo;
	OnTriggerStay(*collideInfo);

	GameObject& temp = GameObjectFactory::CreateBulletBase(GetGameObject().GetSpace());
	Vector3 directionTowardsPlayer = GetGameObject().GetComponent<Transform>().GetPosition() - GameObjectManager::GetInstance().GetGameObjectByID(collideInfo->gameObjectID)->GetComponent<Transform>().GetPosition();

	Vector3 forwardRotation = LookAt(directionTowardsPlayer, GetGameObject().GetComponent<Transform>().GetUp());
	temp.GetComponent<Transform>().SetLocalPosition(GetGameObject().GetComponent<Transform>().GetPosition());
	temp.GetComponent<Transform>().SetLocalRotation(forwardRotation);
	temp.GetComponent<BulletComponent>().bulletImpulse = 1.f;
	temp.GetComponent<BulletComponent>().lifeTime = 5.f;
	temp.GetComponent<BulletComponent>().bulletDamage = 10.f;
	temp.GetComponent<ColliderGroup<SphereCollider>>().colliders[0].SetRadius(1.f);
	temp.GetComponent<MeshHolder>().mesh = MeshBuilder::GetInstance().GenerateOBJ("Enemy Bullet", "OBJ//Game//Bullet//Bullet.obj");	
	temp.GetComponent<MeshHolder>().textureList.textureArray[0] = TextureManager::GetInstance().AddTexture("Bullet Texture", "Image//Game//Bullet//Bullet.tga");
	temp.GetComponent<Rigidbody>().SetMass(0.1f);
	temp.GetComponent<Rigidbody>().AddRelativeForce(GetGameObject().GetComponent<Transform>().GetForward() * temp.GetComponent<BulletComponent>().bulletImpulse, FORCE_MODE::FM_IMPULSE);
	temp.CreateScript<EnemyBulletScript>();

}

bool AIShootingScript::ExtraCondition()
{
	Collider* collideInfo;
	OnTriggerStay(*collideInfo);
	if (GameObjectManager::GetInstance().GetGameObjectByID(collideInfo->gameObjectID)!= nullptr)
		return (GameObjectManager::GetInstance().GetGameObjectByID(collideInfo->gameObjectID)->name == "Player");
	return false;
}


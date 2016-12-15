#include "PlayerShootingScript.h"
#include "../GameObject/GameObjectFactory.h"

PlayerShootingScript::PlayerShootingScript(GameObject& gameObject)
	: ShootingScript(gameObject)
{
}

PlayerShootingScript::~PlayerShootingScript()
{
}

void PlayerShootingScript::ShootBullet()
{
	GameObject& temp = GameObjectFactory::CreateBulletBase(GetGameObject().GetSpace());
	Vector3 forwardRotation = LookAt(GetGameObject().GetComponent<Transform>().GetForward(), GetGameObject().GetComponent<Transform>().GetUp());
	temp.GetComponent<Transform>().SetLocalPosition(GetGameObject().GetComponent<Transform>().GetPosition());
	temp.GetComponent<Transform>().SetLocalRotation(forwardRotation);
	temp.GetComponent<BulletComponent>().bulletImpulse = 50.f;
	temp.GetComponent<BulletComponent>().lifeTime = 5.f;
	temp.GetComponent<ColliderGroup<SphereCollider>>().colliders[0].SetRadius(1.f);
	temp.GetComponent<MeshHolder>().mesh = MeshBuilder::GetInstance().GenerateOBJ("Player's Bullet", "OBJ//Default//Sphere.obj");
	temp.GetComponent<Rigidbody>().SetMass(0.01f);
	temp.GetComponent<Rigidbody>().AddRelativeForce(GetGameObject().GetComponent<Transform>().GetForward() * temp.GetComponent<BulletComponent>().bulletImpulse, FORCE_MODE::FM_IMPULSE);
}

bool PlayerShootingScript::ExtraCondition()
{
	return (InputManager::GetInstance().GetInputInfo().keyDown[INPUT_PLAYERSHOOT]);
}

bool PlayerShootingScript::ManualReload()
{
	return (InputManager::GetInstance().GetInputInfo().keyDown[INPUT_PLAYERRELOAD]);
}
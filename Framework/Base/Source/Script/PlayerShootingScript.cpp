#include "PlayerShootingScript.h"
#include "../GameObject/GameObjectFactory.h"
#include "../Script/PlayerBulletScript.h"

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
	Vector3 forwardRotation = LookAt(GetGameObject().GetComponent<Transform>().GetForward() + GetGameObject().GetComponent<Transform>().GetPosition(), GetGameObject().GetComponent<Transform>().GetUp());

	//float yRotation = Math::RadianToDegree(atan2())
	temp.GetComponent<Transform>().SetLocalPosition(GetGameObject().GetComponent<Transform>().GetPosition());
	temp.GetComponent<Transform>().SetLocalRotation(forwardRotation);
	
	temp.GetComponent<BulletComponent>().bulletImpulse = 1.0f;
	temp.GetComponent<BulletComponent>().lifeTime = 5.f;
	temp.GetComponent<BulletComponent>().bulletDamage = 10.f;
	
	temp.GetComponent<MeshHolder>().mesh = MeshBuilder::GetInstance().GenerateOBJ("Player's Bullet", "OBJ//Game//Bullet.obj");
	temp.GetComponent<MeshHolder>().textureList.textureArray[0] = TextureManager::GetInstance().AddTexture("Bullet Texture", "Image//Game//Bullet//Bullet.tga");
	
	temp.GetComponent<Rigidbody>().SetMass(0.01f);
	temp.GetComponent<Rigidbody>().AddRelativeForce(temp.GetComponent<Transform>().GetForward() * temp.GetComponent<BulletComponent>().bulletImpulse, FORCE_MODE::FM_IMPULSE);
	temp.GetComponent<ColliderGroup<SphereCollider>>().colliders[0].SetRadius(0.1f);
	temp.CreateScript<PlayerBulletScript>();
}

bool PlayerShootingScript::ExtraCondition()
{
	return (InputManager::GetInstance().GetInputInfo().keyDown[INPUT_PLAYERSHOOT]);
}

bool PlayerShootingScript::ManualReload()
{
	return (InputManager::GetInstance().GetInputInfo().keyDown[INPUT_PLAYERRELOAD] && !GetGameObject().GetComponent<RifleComponent>().isReloading);
}
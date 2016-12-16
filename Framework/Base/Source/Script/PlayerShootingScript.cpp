#include "PlayerShootingScript.h"
#include "../GameObject/GameObjectFactory.h"
#include "../Script/PlayerBulletScript.h"
#include "../Script/StickyBullet.h"

PlayerShootingScript::PlayerShootingScript(GameObject& gameObject)
	: ShootingScript(gameObject)
{
	toShoot = toReload = false;
	shootMode = 0;
}

PlayerShootingScript::~PlayerShootingScript()
{
}


void PlayerShootingScript::OnTriggerStay(const Collider& _info)
{
	GameObject* temp  = GameObjectManager::GetInstance().GetGameObjectByID(_info.gameObjectID);
	if (temp->name =="Player")
	{
		if (InputManager::GetInstance().GetInputInfo().keyDown[INPUT_PLAYERSHOOT])
		{
			toShoot = true;
		}
		if ((InputManager::GetInstance().GetInputInfo().keyDown[INPUT_PLAYERRELOAD] && GetGameObject().GetComponent<RifleComponent>().isReloading == false))
		{
			toReload = true;
		}
		if (InputManager::GetInstance().GetInputInfo().keyDown[INPUT_BULLET_STANDARD])
		{
			shootMode = 0;
		}
		if (InputManager::GetInstance().GetInputInfo().keyDown[INPUT_BULLET_STICKY])
		{
			shootMode = 1;
		}
	}
}

void PlayerShootingScript::Update(double deltaTime)
{
	
	ShootingScript::Update(deltaTime);
	toShoot = false;
	toReload = false;
}

void PlayerShootingScript::ShootBullet()
{
	//standard Rounds
	if (shootMode == 0)
	{
		GameObject& temp = GameObjectFactory::CreateBulletBase(GetGameObject().GetSpace());

		Vector3 forwardRotation = LookAt(GetGameObject().GetComponent<Transform>().GetForward() + GetGameObject().GetComponent<Transform>().GetPosition(), GetGameObject().GetComponent<Transform>().GetUp());
		//float yRotation = Math::RadianToDegree(atan2())
		temp.GetComponent<Transform>().SetLocalPosition(GetGameObject().GetComponent<Transform>().GetPosition());
		temp.GetComponent<Transform>().SetLocalRotation(forwardRotation);

		temp.GetComponent<BulletComponent>().bulletImpulse = 1.f;
		temp.GetComponent<BulletComponent>().lifeTime = 5.0f;
		temp.GetComponent<BulletComponent>().bulletDamage = 10.f;

		temp.GetComponent<MeshHolder>().mesh = MeshBuilder::GetInstance().GenerateOBJ("Player Bullet Standard", "OBJ//Game//Bullet.obj");
		//temp.GetComponent<MeshHolder>().mesh = MeshBuilder::GetInstance().GenerateOBJ("Player's Bullet", "OBJ//Default//Sphere.obj");
		temp.GetComponent<MeshHolder>().textureList.textureArray[0] = TextureManager::GetInstance().AddTexture("Bullet Texture", "Image//Game//Bullet//Bullet.tga");

		temp.GetComponent<Rigidbody>().SetMass(0.01f);
		temp.GetComponent<Rigidbody>().AddRelativeForce(temp.GetComponent<Transform>().GetForward() * temp.GetComponent<BulletComponent>().bulletImpulse, FORCE_MODE::FM_IMPULSE);
		temp.GetComponent<ColliderGroup<SphereCollider>>().colliders[0].SetRadius(0.1f);
		//temp.GetComponent<ColliderGroup<SphereCollider>>().colliders[0].SetRadius(0.5f);
		temp.CreateScript<PlayerBulletScript>();
		//temp.CreateScript<StickyBulletScript>();
	}
	//Sticky Rounds
	else if (shootMode == 1)
	{
		GameObject& temp = GameObjectFactory::CreateStickyBullet(GetGameObject().GetSpace());
		Vector3 forwardRotation = LookAt(GetGameObject().GetComponent<Transform>().GetForward() + GetGameObject().GetComponent<Transform>().GetPosition(), GetGameObject().GetComponent<Transform>().GetUp());
		temp.GetComponent<Transform>().SetLocalPosition(GetGameObject().GetComponent<Transform>().GetPosition());
		temp.GetComponent<Transform>().SetLocalRotation(forwardRotation);

		temp.GetComponent<BulletComponent>().bulletImpulse = 1.f;
		temp.GetComponent<BulletComponent>().lifeTime = 5.0f;
		temp.GetComponent<BulletComponent>().bulletDamage = 10.f;

		temp.GetComponent<MeshHolder>().mesh = MeshBuilder::GetInstance().GenerateOBJ("Player Bullet Sticky", "OBJ//Default//Sphere.obj");
		temp.GetComponent<MeshHolder>().textureList.textureArray[0] = TextureManager::GetInstance().AddTexture("Bullet Texture", "Image//Game//Bullet//Bullet.tga");

		temp.GetComponent<Rigidbody>().SetMass(0.01f);
		temp.GetComponent<Rigidbody>().AddRelativeForce(temp.GetComponent<Transform>().GetForward() * temp.GetComponent<BulletComponent>().bulletImpulse, FORCE_MODE::FM_IMPULSE);
		temp.GetComponent<ColliderGroup<SphereCollider>>().colliders[0].SetRadius(0.5f);
		temp.CreateScript<StickyBulletScript>();
	}

	
}

bool PlayerShootingScript::ExtraCondition()
{
	return toShoot;
}

bool PlayerShootingScript::ManualReload()
{
	return toReload;
}
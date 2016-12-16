#include "PlayerShootingScript.h"
#include "../GameObject/GameObjectFactory.h"
#include "../Script/PlayerBulletScript.h"

PlayerShootingScript::PlayerShootingScript(GameObject& gameObject)
	: ShootingScript(gameObject)
{
	toShoot = toReload = false;
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
	cout << "Shooting Bullet" << endl;
	GameObject& temp = GameObjectFactory::CreateBulletBase(GetGameObject().GetSpace());
	Vector3 forwardRotation = LookAt(GetGameObject().GetComponent<Transform>().GetForward() + GetGameObject().GetComponent<Transform>().GetPosition(), GetGameObject().GetComponent<Transform>().GetUp());

	//float yRotation = Math::RadianToDegree(atan2())
	temp.GetComponent<Transform>().SetLocalPosition(GetGameObject().GetComponent<Transform>().GetPosition());
	temp.GetComponent<Transform>().SetLocalRotation(forwardRotation);
	
	temp.GetComponent<BulletComponent>().bulletImpulse = 1.f;
	temp.GetComponent<BulletComponent>().lifeTime = 5.0f;
	temp.GetComponent<BulletComponent>().bulletDamage = 10.f;

	temp.GetComponent<MeshHolder>().mesh = MeshBuilder::GetInstance().GenerateOBJ("Player's Bullet", "OBJ//Game//Bullet.obj");
	//temp.GetComponent<MeshHolder>().mesh = MeshBuilder::GetInstance().GenerateOBJ("Player's Bullet", "OBJ//Default//Sphere.obj");
	temp.GetComponent<MeshHolder>().textureList.textureArray[0] = TextureManager::GetInstance().AddTexture("Bullet Texture", "Image//Game//Bullet//Bullet.tga");
	
	temp.GetComponent<Rigidbody>().SetMass(0.01f);
	temp.GetComponent<Rigidbody>().AddRelativeForce(temp.GetComponent<Transform>().GetForward() * temp.GetComponent<BulletComponent>().bulletImpulse, FORCE_MODE::FM_IMPULSE);
	temp.GetComponent<ColliderGroup<SphereCollider>>().colliders[0].SetRadius(0.1f);
	temp.CreateScript<PlayerBulletScript>();
	cout << "Bullet ID: " << temp.GetID() << endl;
	cout << "Bullet Position: " << temp.GetComponent<Transform>().GetPosition() << endl;
}

bool PlayerShootingScript::ExtraCondition()
{
	return toShoot;
}

bool PlayerShootingScript::ManualReload()
{
	return toReload;
}
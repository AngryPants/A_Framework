#include "AIShootingScript.h"
#include "../GameObject/GameObjectFactory.h"
#include "../Script/EnemyBulletScript.h"


AIShootingScript::AIShootingScript(GameObject& gameObject) : ShootingScript(gameObject)
{
	toShoot = false;
}

AIShootingScript::~AIShootingScript()
{

}

void AIShootingScript::OnTriggerStay(const Collider& _collider)
{
	if (GameObjectManager::GetInstance().GetGameObjectByID(_collider.gameObjectID)->name == "Player")
	{
		toShoot = true;
		playerPosition = GameObjectManager::GetInstance().GetGameObjectByID(_collider.gameObjectID)->GetComponent<Transform>().GetPosition();
	}
}

void AIShootingScript::ShootBullet()
{
	GameObject& temp = GameObjectFactory::CreateBulletBase(GetGameObject().GetSpace());
	Transform& myTransform = GetGameObject().GetComponent<Transform>();
	//float yRotation = Math::RadianToDegree(atan2(playerPosition.z - myTransform.GetPosition().z, playerPosition.x - myTransform.GetPosition().x));
	//temp.GetComponent<Transform>().SetLocalRotation(0,yRotation,0);
	temp.GetComponent<Transform>().SetLocalRotation(LookAt(playerPosition,Vector3(0,1,0)));
	temp.GetComponent<Transform>().SetLocalPosition(GetGameObject().GetComponent<Transform>().GetPosition());
	temp.GetComponent<BulletComponent>().bulletImpulse = 1.f;
	temp.GetComponent<BulletComponent>().lifeTime = 5.f;
	temp.GetComponent<BulletComponent>().bulletDamage = 10.f;
	temp.GetComponent<ColliderGroup<SphereCollider>>().colliders[0].SetRadius(1.f);
	temp.GetComponent<MeshHolder>().mesh = MeshBuilder::GetInstance().GenerateOBJ("Enemy Bullet", "OBJ//Default//Sphere.obj");	
	temp.GetComponent<MeshHolder>().textureList.textureArray[0] = TextureManager::GetInstance().AddTexture("Bullet Texture", "Image//Game//Bullet//Bullet.tga");
	temp.GetComponent<Rigidbody>().SetMass(0.03f);
	temp.GetComponent<Rigidbody>().useGravity = false;
	temp.GetComponent<Rigidbody>().AddRelativeForce(temp.GetComponent<Transform>().GetForward() * temp.GetComponent<BulletComponent>().bulletImpulse, FORCE_MODE::FM_IMPULSE);
	temp.CreateScript<EnemyBulletScript>();

}

void AIShootingScript::Update(double deltaTime)
{
	ShootingScript::Update(deltaTime);
	toShoot = false;

}

bool AIShootingScript::ExtraCondition()
{
	return toShoot;
}


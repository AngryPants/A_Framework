#ifndef GAMEOBJECT_FACTORY_H
#define GAMEOBJECT_FACTORY_H

#include "GameObject.h"
#include "GameObjectManager.h"

//Include Systems
#include "../Texture/TextureManager.h"
#include "../Mesh/MeshBuilder.h"

//Include Scripts
#include "../Script/HealthScript.h"
#include "../Script/BulletScript.h"
#include "../Script/PlayerShootingScript.h"
#include "../Script/AIShootingScript.h"
#include "../Script/AIMovementScript.h"

//Include Components
#include "../Component/Rendering/Camera.h"
#include "../Component/Rendering/Light.h"
#include "../Component/Rendering/MeshRenderer.h"
#include "../Component/Rendering/MeshHolder.h"
#include "../Component/Rendering/LODMeshHolder.h"
#include "../Component/Rendering/SpriteRenderer.h"
#include "../Component/Rendering/TextRenderer.h"
#include "../Component/Physics/Transform.h"
#include "../Component/Physics/Rigidbody.h"
#include "../Component/Colliders/BoxCollider.h"
#include "../Component/Colliders/SphereCollider.h"

//////////////AGDEVGAME/////////////////////
#include "../Component/Game/Bullet.h"
#include "../Component/Game/Health.h"
#include "../Component/Game/Rifle.h"
#include "../Component/Game/Waypoint.h"


class GameObjectFactory {

private:
	//Constructor(s) & Destructor
	GameObjectFactory() {}
	virtual ~GameObjectFactory() {}

	//private functions
	static GameObject& CreateRifleBase(const string& space, const string& name = "rifle")
	{
		GameObject& go = GameObjectManager::GetInstance().CreateGameObject(space, name);
		go.AddComponent<BoxCollider>();
		go.AddComponent<RifleComponent>();
		go.AddComponent<LODMeshHolder>();
		go.GetComponent<RifleComponent>().clipSize = 30;
		go.GetComponent<RifleComponent>().currentClipSize = 30;
		go.GetComponent<RifleComponent>().magazineSize = 150;
		go.GetComponent<RifleComponent>().reloadTime = 5.f;
		go.GetComponent<RifleComponent>().SetRateOfFire(10.f);

		return go;
	}

public:
	//Interface Function(s)
	static GameObject& CreateCamera(const string& space, const string& name = "Camera") {
		GameObject& go = GameObjectManager::GetInstance().CreateGameObject(space, name);
		go.AddComponent<Camera>();

		return go;
	}

	static GameObject& CreateLight(const string& space, const string& name = "Light") {
		GameObject& go = GameObjectManager::GetInstance().CreateGameObject(space, name);
		go.AddComponent<Light>();

		return go;
	}

	static GameObject& CreateEmpty(const string& space, const string& name = "GameObject") {
		return GameObjectManager::GetInstance().CreateGameObject(space, name);
	}

	static GameObject& CreateCube(const string& space, const string& name = "Cube") {
		GameObject& go = GameObjectManager::GetInstance().CreateGameObject(space, name);
		go.AddComponent<MeshHolder>().mesh = MeshBuilder::GetInstance().GenerateOBJ("Default Cube", "OBJ//Default//Cube.obj");

		return go;
	}

	static GameObject& CreateSphere(const string& space, const string& name = "Sphere") {
		GameObject& go = GameObjectManager::GetInstance().CreateGameObject(space, name);
		go.AddComponent<MeshHolder>().mesh = MeshBuilder::GetInstance().GenerateOBJ("Default Sphere", "OBJ//Default//Sphere.obj");
		
		return go;
	}

	static GameObject& CreateQuad(const string& space, const string& name = "Quad") {
		GameObject& go = GameObjectManager::GetInstance().CreateGameObject(space, name);
		go.AddComponent<MeshHolder>().mesh = MeshBuilder::GetInstance().GenerateQuad("Default Quad");

		return go;
	}

	static GameObject& CreatePlane(const string& space, const string& name = "Plane") {
		GameObject& go = GameObjectManager::GetInstance().CreateGameObject(space, name);
		go.AddComponent<MeshHolder>().mesh = MeshBuilder::GetInstance().GenerateOBJ("Default Plane", "OBJ//Default//Plane.obj");

		return go;
	}

	static GameObject& CreateCylinder(const string& space,const string& name = "Cylinder") {
		GameObject& go = GameObjectManager::GetInstance().CreateGameObject(space,name);
		go.AddComponent<MeshHolder>().mesh = MeshBuilder::GetInstance().GenerateOBJ("Default Cylinder","OBJ//Default//Cylinder.obj");

		return go;
	}

	static GameObject& CreateEnemyBase(const string& space, const string& name = "Enemy")
	{
		GameObject& go = GameObjectManager::GetInstance().CreateGameObject(space, name);
		go.AddComponent<HealthComponent>();
		go.AddComponent<BoxCollider>(); 
		go.AddComponent<LODMeshHolder>();

		return go;
	}

	static GameObject& CreateBulletBase(const string& space, const string& name = "bullet")
	{
		GameObject& go = GameObjectManager::GetInstance().CreateGameObject(space, name);
		go.AddComponent<Rigidbody>();
		go.AddComponent<SphereCollider>();
		go.AddComponent<BulletComponent>();
		go.AddComponent<MeshHolder>();

		return go;
	}

	static GameObject& CreateWayPointBase(const string& space, const string& name = "waypoint")
	{
		GameObject& go = GameObjectManager::GetInstance().CreateGameObject(space, name);
		go.AddComponent<WayPointComponent>(); 
		go.AddComponent<SphereCollider>();

		return go;
	}

	static GameObject& CreateEnemyRifle(const string& space, const string& name = "Enemy_Rifle")
	{
		GameObject& go = CreateRifleBase(space, name);
		go.CreateScript<AIShootingScript>();
		return go;
	}

	static GameObject& CreateEquippableRifle(const string& space, const string& name = "Normal_Rifle")
	{
		GameObject& go = CreateRifleBase(space, name);
		go.CreateScript<PlayerShootingScript>();

		return go;
	}

	static GameObject& CreateDefaultEnemy(const string& space, const string& name = "Enemy")
	{
		GameObject& go = CreateEnemyBase(space,name);
		go.CreateScript<AIShootingScript>();

		//Create Rifle for enemy
		GameObject& enemyRifle = CreateEnemyRifle(space, name);
		enemyRifle.SetParent(go);
		return go;
	}

	static GameObject& CreateDefaultMovingEnemy(const string& space, const string& name = "Enemy")
	{
		GameObject& go = CreateDefaultEnemy(space, name);
		go.GetComponent<Transform>().SetLocalPosition(0, 1, 0);
		AIMovementScript * waypoint = go.CreateScript<AIMovementScript>();
		//waypoint->CreateWayPoint(Vector3(20, 1, 20), 1.f);
		//waypoint->CreateWayPoint(Vector3(-20, 1, 20), 1.f);
		//waypoint->CreateWayPoint(Vector3(20, 1, -20), 1.f);
		waypoint->CreateWayPoint(Vector3(-20, 1, -20), 1.f);
		return go;
	}
};

#endif
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
	static GameObject& CreateRifleBase(const string& space, const string& name = "rifle", const int& clipSize = 5, const int& magazineSize = 30, const float& reloadTime = 2.f, const float& rateOfFire = 10.f)
	{
		GameObject& go = GameObjectManager::GetInstance().CreateGameObject(space, name);
		go.AddComponent<RifleComponent>();
		go.AddComponent<LODMeshHolder>();/*.SetLODMesh("OBJ//Default//Cube.obj", "OBJ//Default//Cube.obj", "OBJ//Default//Cube.obj");
		go.GetComponent<LODMeshHolder>().SetLODTextures("","","");*/

		go.GetComponent<RifleComponent>().clipSize = clipSize;
		go.GetComponent<RifleComponent>().magazineSize = magazineSize;
		go.GetComponent<RifleComponent>().reloadTime = reloadTime;
		go.GetComponent<RifleComponent>().SetRateOfFire(rateOfFire);
		
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

	static GameObject& CreateEnemy(const string& space, const string& name = "Enemy")
	{
		GameObject& go = GameObjectManager::GetInstance().CreateGameObject(space, name);
		go.AddComponent<HealthComponent>().SetHealth(10);
		go.AddComponent<RifleComponent>();
		go.AddComponent<BoxCollider>().centre = go.GetComponent<Transform>().GetPosition();
		go.CreateScript<ShootingScript>();
		go.CreateScript<HealthScript>();
		go.CreateScript<AIMovementScript>();
		AIMovementScript* waypoints = go.CreateScript<AIMovementScript>();
		waypoints->CreateWayPoint(Vector3(10, 0, 10), 5.f);
		waypoints->CreateWayPoint(Vector3(-10, 0, -10), 5.f);
		waypoints->CreateWayPoint(Vector3(-10, 0, 10), 5.f);
		waypoints->CreateWayPoint(Vector3(10, 0, -10), 5.f);

		return go;
	}

	static GameObject& CreateEnemyRifle(const string& space, const string& name = "Enemy_Rifle")
	{
		GameObject& go = CreateRifleBase(space,name);
		go.CreateScript<AIShootingScript>();

		return go;
	}

	static GameObject& CreateEquippableRifle(const string& space, const string& name = "Normal_Rifle")
	{
		GameObject& go = CreateRifleBase(space, name);
		go.CreateScript<PlayerShootingScript>();

		return go;
	}

	static GameObject& CreateBullet(const string& space, const string& name, const string& filePath, const float& lifeTime, const float& bulletSpeed, const float& bulletSize)
	{
		GameObject& go = GameObjectManager::GetInstance().CreateGameObject(space, name);
		go.AddComponent<Rigidbody>();
		go.AddComponent<SphereCollider>();
		go.AddComponent<BulletComponent>();
		go.AddComponent<MeshHolder>().mesh = MeshBuilder::GetInstance().GenerateOBJ(name, filePath);

		go.GetComponent<BulletComponent>().bulletSize = bulletSize;
		go.GetComponent<BulletComponent>().bulletSpeed = bulletSpeed;
		go.GetComponent<BulletComponent>().lifeTime = lifeTime;

		go.CreateScript<BulletScript>();

		return go;
	}

	static GameObject& CreateWayPoint(const string& space, const string& name = "waypoint", const Vector3& waypointPosition = Vector3(0,0,0), const float& radius = 5.f)
	{
		GameObject& go = GameObjectManager::GetInstance().CreateGameObject(space, name);
		go.AddComponent<WayPointComponent>();
		go.AddComponent<SphereCollider>().isTrigger = true;
		go.AddComponent<SphereCollider>().centre = waypointPosition;
		go.AddComponent<SphereCollider>().radius = radius;

		return go;
	}

};

#endif
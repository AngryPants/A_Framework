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
#include "../AGDEV/CutsceneTriggerScript.h"

//Include Components
#include "../Component/Rendering/Camera.h"
#include "../Component/Rendering/Light.h" 
#include "../Component/Physics/Transform.h"
#include "../Component/Physics/Rigidbody.h" 
#include "../Component/Rendering/MeshRenderer.h"
#include "../Component/Rendering/MeshHolder.h"
#include "../Component/Rendering/LODMeshHolder.h"
#include "../Component/Rendering/SpriteRenderer.h"
#include "../Component/Rendering/TextRenderer.h" 
#include "../Component/Physics/Collision/ColliderGroup.h"

//Include Colliders
#include "../Collider/SphereCollider.h"
#include "../Collider/AABBCollider.h"
#include "../Component/Rendering/TextRenderer.h"
#include "../Component/Physics/Transform.h"
#include "../Component/Physics/Rigidbody.h"

//////////////AGDEVGAME/////////////////////
#include "../Component/Game/Bullet.h"
#include "../Component/Game/Health.h"
#include "../Component/Game/Rifle.h"
#include "../Component/Game/Waypoint.h"
#include "../AGDEV/AIChaseScript.h"
#include "../../Common/Lua/LuaManager.h"

class GameObjectFactory {

private:
	//Constructor(s) & Destructor
	GameObjectFactory() {}
	virtual ~GameObjectFactory() {}

	//private functions
	static GameObject& CreateRifleBase(const string& space, const string& name = "rifle")
	{
		GameObject& go = GameObjectManager::GetInstance().CreateGameObject(space, name);
		go.AddComponent<LODMeshHolder>();
		go.AddComponent<RifleComponent>();

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

	static GameObject& CreateText(const string& space,const string& name = "Cylinder") {
		GameObject& go = GameObjectManager::GetInstance().CreateGameObject(space,name);
		go.AddComponent<TextRenderer>();

		return go;
	}

	static GameObject& CreateGrass(const string& space, const string& name = "Grass") {
		GameObject& go = GameObjectManager::GetInstance().CreateGameObject(space,name);
		go.AddComponent<LODMeshHolder>().SetLODMesh(MeshBuilder::GetInstance().GenerateOBJ("Grass Low", "OBJ//Game//Grass//Low.obj"),
													MeshBuilder::GetInstance().GenerateOBJ("Grass Mid", "OBJ//Game//Grass//Mid.obj"),
													MeshBuilder::GetInstance().GenerateOBJ("Grass High", "OBJ//Game//Grass//High.obj")),
		go.GetComponent<LODMeshHolder>().textureList[LODMeshHolder::DETAIL_LEVEL::LOW_DETAILS].textureArray[0] = TextureManager::GetInstance().AddTexture("Grass", "Image//Game//Model_Textures//Grass//Grass.tga");
		go.GetComponent<LODMeshHolder>().textureList[LODMeshHolder::DETAIL_LEVEL::MID_DETAILS].textureArray[0] = TextureManager::GetInstance().AddTexture("Grass", "Image//Game//Model_Textures//Grass//Grass.tga");
		go.GetComponent<LODMeshHolder>().textureList[LODMeshHolder::DETAIL_LEVEL::HIGH_DETAILS].textureArray[0] = TextureManager::GetInstance().AddTexture("Grass", "Image//Game//Model_Textures//Grass//Grass.tga");

		return go;
	}

	static GameObject& CreatePlatform(const string& space, const string& name = "Platform") {
		GameObject& go = GameObjectManager::GetInstance().CreateGameObject(space,name);
		go.AddComponent<LODMeshHolder>().SetLODMesh(MeshBuilder::GetInstance().GenerateOBJ("Platform Low", "OBJ//Game//Platform//Low.obj"),
													MeshBuilder::GetInstance().GenerateOBJ("Platform Mid", "OBJ//Game//Platform//Mid.obj"),
													MeshBuilder::GetInstance().GenerateOBJ("Platform High", "OBJ//Game//Platform//High.obj")),
		go.GetComponent<LODMeshHolder>().textureList[LODMeshHolder::DETAIL_LEVEL::LOW_DETAILS].textureArray[0] = TextureManager::GetInstance().AddTexture("Platform", "Image//Game//Model_Textures//Platform.tga");
		go.GetComponent<LODMeshHolder>().textureList[LODMeshHolder::DETAIL_LEVEL::MID_DETAILS].textureArray[0] = TextureManager::GetInstance().AddTexture("Platform", "Image//Game//Model_Textures//Platform.tga");
		go.GetComponent<LODMeshHolder>().textureList[LODMeshHolder::DETAIL_LEVEL::HIGH_DETAILS].textureArray[0] = TextureManager::GetInstance().AddTexture("Platform", "Image//Game//Model_Textures//Platform.tga");
		go.AddComponent<ColliderGroup<AABBCollider>>().colliders[0].size.Set(4, 5, 6);
		go.AddComponent<ColliderGroup<AABBCollider>>().colliders[0].isGround = true;
		go.AddComponent<ColliderGroup<AABBCollider>>().colliders[0].centre.y = -2.5;

		return go;
	}

	static GameObject& CreateBulletBase(const string& space, const string& name = "bullet")
	{
		GameObject& go = GameObjectManager::GetInstance().CreateGameObject(space, name);
		go.AddComponent<Rigidbody>();
		go.AddComponent<ColliderGroup<SphereCollider>>(); 
		go.AddComponent<BulletComponent>();
		go.AddComponent<MeshHolder>();

		return go;
	}

	static GameObject& CreateStickyBullet(const string& space, const string& name = "bullet")
	{
		GameObject& go = GameObjectManager::GetInstance().CreateGameObject(space, name);
		go.AddComponent<Rigidbody>();
		go.AddComponent<ColliderGroup<SphereCollider>>();
		go.AddComponent<BulletComponent>();
		go.AddComponent<MeshHolder>();

		return go;
	}

	static GameObject& CreateWayPointBase(const string& space, const string& name = "Waypoint")
	{
		GameObject& go = GameObjectManager::GetInstance().CreateGameObject(space, name);
		go.AddComponent<WayPointComponent>();
		go.AddComponent<ColliderGroup<SphereCollider>>(); 

		return go;
	}

	static GameObject& CreateEnemyRifle(const string& space, const string& name = "Enemy_Rifle")
	{
		GameObject& go = CreateRifleBase(space, name);
		// collider 0 is the hitbox for enemy Range
		go.AddComponent<ColliderGroup<SphereCollider>>().colliders[0].SetRadius(10.f);
		go.GetComponent<ColliderGroup<SphereCollider>>().colliders[0].isTrigger = true;
		go.GetComponent<RifleComponent>().clipSize = 10;
		go.GetComponent<RifleComponent>().currentClipSize = 10;
		go.GetComponent<RifleComponent>().magazineSize = 80;
		go.GetComponent<RifleComponent>().reloadTime = 1.f;
		go.GetComponent<RifleComponent>().SetRateOfFire(1.f);
		go.CreateScript<AIShootingScript>();

		return go;
	}

	static GameObject& CreateEquippableRifle(const string& space, const string& name = "Normal_Rifle")
	{
		GameObject& go = CreateRifleBase(space, name);
		// collider 0 is the hitbox for picking up the gun
		go.AddComponent<ColliderGroup<SphereCollider>>().colliders[0].SetRadius(2.f);
		go.AddComponent<ColliderGroup<SphereCollider>>().colliders[0].isTrigger = true;
		go.GetComponent<RifleComponent>().clipSize = 80;
		go.GetComponent<RifleComponent>().currentClipSize = 80;
		go.GetComponent<RifleComponent>().magazineSize = 250;
		go.GetComponent<RifleComponent>().reloadTime = 0.5f;
		go.GetComponent<RifleComponent>().SetRateOfFire(8.f);
		go.CreateScript<PlayerShootingScript>();

		return go;
	}

	static GameObject& CreateEnemyBase(const string& space, const string& name = "Enemy")
	{
		GameObject& go = GameObjectManager::GetInstance().CreateGameObject(space, name);
		go.AddComponent<Rigidbody>().useGravity = false;
		go.AddComponent<HealthComponent>();
		go.AddComponent<ColliderGroup<SphereCollider>>();
		go.AddComponent<LODMeshHolder>();
		go.GetComponent<ColliderGroup<SphereCollider>>().CreateColliders(3);
		go.GetComponent<ColliderGroup<SphereCollider>>().colliders[0].SetRadius(2.f);
		//Health Script
		go.CreateScript<HealthScript>();
		return go;
	}

	static GameObject& CreateDefaultEnemy(const string& space)
	{
		GameObject& go = CreateEnemyBase(space);
		//Enemy AttackRange
		go.GetComponent<ColliderGroup<SphereCollider>>().colliders[1].SetRadius(10.f);
		go.GetComponent<ColliderGroup<SphereCollider>>().colliders[1].isTrigger = true;

		//Create Rifle for enemy
		/*Mesh* rifleMeshLowLOD = MeshBuilder::GetInstance().GenerateOBJ("Rifle Mesh Low LOD", "OBJ//Game//M4A1//M4A1.obj");
		Mesh* rifleMeshMidLOD = MeshBuilder::GetInstance().GenerateOBJ(" Rifle Mesh Mid LOD", "OBJ//Game//M4A1//M4A1.obj");
		Mesh* rifleMeshHighLOD = MeshBuilder::GetInstance().GenerateOBJ("Rifle Mesh High LOD", "OBJ//Game//M4A1//M4A1.obj");
		
		enemyRifle.GetComponent<LODMeshHolder>().SetLODMesh(rifleMeshLowLOD, rifleMeshMidLOD, rifleMeshHighLOD);
		enemyRifle.GetComponent<LODMeshHolder>().textureList[LODMeshHolder::DETAIL_LEVEL::HIGH_DETAILS].textureArray[0] = TextureManager::GetInstance().AddTexture("Rifle Texture", "Image//Game//M4A1//M4A1.tga");
		enemyRifle.GetComponent<LODMeshHolder>().textureList[LODMeshHolder::DETAIL_LEVEL::MID_DETAILS].textureArray[0] = TextureManager::GetInstance().AddTexture("Rifle Texture", "Image//Game//M4A1//M4A1.tga");
		enemyRifle.GetComponent<LODMeshHolder>().textureList[LODMeshHolder::DETAIL_LEVEL::LOW_DETAILS].textureArray[0] = TextureManager::GetInstance().AddTexture("Rifle Texture", "Image//Game//M4A1//M4A1.tga");
		enemyRifle.SetParent(go);*/

		GameObject& enemyRifle = CreateEnemyRifle(space);
		enemyRifle.GetComponent<RifleComponent>().isHeld = true;
		enemyRifle.SetParent(go);

		return go;
	}

	static GameObject& CreateDefaultMovingEnemy(const string& space)
	{
		GameObject& go = CreateDefaultEnemy(space);
		//Collider 2 is for Checking with waypoints Movement
		go.GetComponent<ColliderGroup<SphereCollider>>().colliders[2].SetRadius(1.f);
		go.GetComponent<ColliderGroup<SphereCollider>>().colliders[2].isTrigger = true;
		go.GetComponent<Transform>().SetLocalPosition(0, 1, 0);
		AIMovementScript * waypoint = go.CreateScript<AIMovementScript>();

		//should be done outside
		/*Vector3 waypoint1 = LuaManager::GetInstance().GetLuaFile("Waypoint")->GenerateWayPoints(0, 50);
		Vector3 waypoint2 = LuaManager::GetInstance().GetLuaFile("Waypoint")->GenerateWayPoints(0, 100);
		Vector3 waypoint3 = LuaManager::GetInstance().GetLuaFile("Waypoint")->GenerateWayPoints(0, 150);
		Vector3 waypoint4 = LuaManager::GetInstance().GetLuaFile("Waypoint")->GenerateWayPoints(0, 200);
		*/

		Vector3 waypoint1 = LuaManager::GetInstance().GetLuaFile("Waypoint")->GetWayPoint(1);
		Vector3 waypoint2 = LuaManager::GetInstance().GetLuaFile("Waypoint")->GetWayPoint(2);
		Vector3 waypoint3 = LuaManager::GetInstance().GetLuaFile("Waypoint")->GetWayPoint(3);
		Vector3 waypoint4 = LuaManager::GetInstance().GetLuaFile("Waypoint")->GetWayPoint(4);

		waypoint->CreateWayPoint(waypoint1, 5.f);
		waypoint->CreateWayPoint(waypoint2, 5.f);
		waypoint->CreateWayPoint(waypoint3, 5.f);
		waypoint->CreateWayPoint(waypoint4, 5.f);
		waypoint->LinkWayPoint();

		//Collider 3 is for Enemy Chase Range
		go.GetComponent<ColliderGroup<SphereCollider>>().colliders[3].SetRadius(15.f);
		go.GetComponent<ColliderGroup<SphereCollider>>().colliders[3].isTrigger = true;
		go.CreateScript<AIChaseScript>();
		return go;
	}

	static GameObject& CreateCutsceneTrigger(const string& space, const string& name = "Cutscene Trigger") {
		GameObject& go = GameObjectManager::GetInstance().CreateGameObject(space, name);
		go.AddComponent<ColliderGroup<SphereCollider>>().colliders[0].SetRadius(20.0f);
		go.AddComponent<ColliderGroup<SphereCollider>>().colliders[0].isTrigger = true;
		go.CreateScript<CutsceneTriggerScript>();

		return go;
	}
	
};

#endif
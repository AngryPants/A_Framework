#include "SceneAsn1.h"
#include "../Graphics/GraphicsManager.h"
#include "../Graphics/RenderHelper.h"
#include "../Application/Application.h"
#include "../Input/InputManager.h"
#include "../Mesh/MeshBuilder.h"
#include "../Texture/TextureManager.h"
#include "../Texture/TextureList.h"
#include "../Systems/Rendering//RenderSystem.h"
#include "../Systems/SpatialPartition/SPSystem.h"
#include "../Systems/Physics/PhysicsSystem.h"

//Include Scripts
#include "RotateScript.h"
#include "ScaleScript.h"
#include "PlayerCameraScript.h"
#include "PlayerMovementScript.h"
#include "../Script//PlayerActionScript.h"
#include "TranslateScript.h"
#include "PivotScript.h"
#include "DebugControlsScript.h" 
#include "TriggerTestScript.h" 
#include "../Script//PlayerShootingScript.h"
#include "../Component/Game/PlayerData.h"
#include "../Script/PlayerHealthScript.h"

//Constructor(s) & Destructor
SceneAsn1::SceneAsn1(const string& name) : Scene(name) {
}

SceneAsn1::~SceneAsn1() {
}

//Interface Function(s)
void SceneAsn1::Init() {
	//Initialise some GraphicsManager stuff.
	GraphicsManager::GetInstance().Enable<GraphicsManager::MODE::BLENDING>();
	GraphicsManager::GetInstance().Enable<GraphicsManager::MODE::CULLING>();
	GraphicsManager::GetInstance().Enable<GraphicsManager::MODE::DEPTH_TEST>();
	//GraphicsManager::GetInstance().Disable<GraphicsManager::MODE::CULLING>();

	//Initialise the Shader.
	RenderHelper::GetInstance().LoadShader(SHADERS::PHONG);
	RenderHelper::GetInstance().EnableLight(true);
	RenderHelper::GetInstance().SetNumLights(8);
	RenderHelper::GetInstance().EnableFog(false);
	RenderHelper::GetInstance().SetAlphaDiscardValue(0.1f);

	//Set Skybox Textures
	skybox.textureLists[Skybox::TEXTURES::SKYBOX_TOP].textureArray[0] = TextureManager::GetInstance().AddTexture("Skybox Top", "Image//Skybox//Mountains//Top.tga");
	skybox.textureLists[Skybox::TEXTURES::SKYBOX_BOTTOM].textureArray[0] = TextureManager::GetInstance().AddTexture("Skybox Bottom", "Image//Skybox//Mountains//Bottom.tga");
	skybox.textureLists[Skybox::TEXTURES::SKYBOX_BACK].textureArray[0] = TextureManager::GetInstance().AddTexture("Skybox Back", "Image//Skybox//Mountains//Back.tga");
	skybox.textureLists[Skybox::TEXTURES::SKYBOX_LEFT].textureArray[0] = TextureManager::GetInstance().AddTexture("Skybox Left", "Image//Skybox//Mountains//Left.tga");
	skybox.textureLists[Skybox::TEXTURES::SKYBOX_FRONT].textureArray[0] = TextureManager::GetInstance().AddTexture("Skybox Front", "Image//Skybox//Mountains//Front.tga");
	skybox.textureLists[Skybox::TEXTURES::SKYBOX_RIGHT].textureArray[0] = TextureManager::GetInstance().AddTexture("Skybox Right", "Image//Skybox//Mountains//Right.tga");

	//SpatialPartition 
	int xGridSize = 50; int yGridSize = 50; int zGridSize = 50;
	int xNumGrid = 50; int yNumGrid = 5; int zNumGrid = 50;
	//int xNumGrid = 0; int yNumGrid = 0; int zNumGrid = 0;
	SpatialPartitionSystem::GetInstance().CreateSpatialPartition(name)->Set(xGridSize, yGridSize, zGridSize, xNumGrid, yNumGrid, zNumGrid, 0, ((yNumGrid >> 1) - 1) * yGridSize, 0);

	//Player
	player = &GameObjectFactory::CreateEmpty(name, "Player");
	player->CreateScript<PlayerMovementScript>();
	player->CreateScript<DebugControlsScript>();
	player->CreateScript<PlayerHealthScript>();
	player->CreateScript<PlayerActionScript>();
	player->AddComponent<HealthComponent>();
	player->GetComponent<HealthComponent>().SetMaxHealth(250);
	player->GetComponent<HealthComponent>().SetHealth(250);
	player->AddComponent<Rigidbody>().elasticity = 0.0f;
	//player->AddComponent<ColliderGroup<AABBCollider>>().colliders[0];
	player->AddComponent <PlayerDataComponent>();
	player->AddComponent<ColliderGroup<SphereCollider>>().CreateColliders(1);
	player->GetComponent<ColliderGroup<SphereCollider>>().colliders[0].SetRadius(5.f);
	player->GetComponent<ColliderGroup<SphereCollider>>().colliders[1].SetRadius(2.f);
	player->GetComponent<ColliderGroup<SphereCollider>>().colliders[1].isTrigger = true;
	//player->AddComponent<MeshHolder>().mesh = MeshBuilder::GetInstance().GenerateOBJ("Player Sphere", "OBJ//Default//Sphere.obj");

	//Camera
	GameObject* camera = &GameObjectFactory::CreateCamera(name, "Player Camera");
	camera->SetParent(*player);
	camera->GetComponent<Transform>().SetLocalPosition(0, 1.7f, 0);
	camera->CreateScript<PlayerCameraScript>();

	//Lights
	GameObject* light = &GameObjectFactory::CreateLight(name);
	light->GetComponent<Light>().type = Light::LIGHT_TYPE::LIGHT_DIRECTIONAL;
	light->GetComponent<Light>().power = 1.0f;
	light->GetComponent<Transform>().SetLocalPosition(0, 0, 5);
	light->GetComponent<Transform>().SetLocalRotation(45, 0, 0);

	GameObject* ground = &GameObjectFactory::CreatePlane(name, "Ground");
	ground->GetComponent<MeshHolder>().mesh = MeshBuilder::GetInstance().GeneratePlane("Ground", Color(1, 1, 1), 1.0f, 500, 500);
	ground->GetComponent<MeshHolder>().mesh->material.kDiffuse.Set(0.7f, 0.7f, 0.7f);
	ground->GetComponent<Transform>().SetLocalPosition(0, 0, 0);
	ground->GetComponent<Transform>().SetLocalScale(1000, 1, 1000);
	ground->useSpatialPartition = false;
	ground->GetComponent<MeshHolder>().textureList.textureArray[0] = TextureManager::GetInstance().AddTexture("Ground Grass", "Image//Game//Environment//Grass.tga");
	ground->AddComponent<ColliderGroup<AABBCollider>>().colliders[0].centre.Set(0, -250.0f, 0);
	ground->GetComponent<ColliderGroup<AABBCollider>>().colliders[0].size.Set(1000, 500, 1000);
	ground->GetComponent<ColliderGroup<AABBCollider>>().colliders[0].isGround = true;

	//Background Mountain
	{
		vector<unsigned char> heightMap;
		GameObject* mountain = &GameObjectFactory::CreateEmpty(name, "Mountain");
		mountain->GetComponent<Transform>().SetLocalPosition(0, -20, 500);
		mountain->GetComponent<Transform>().SetLocalScale(1000, 70, 500);
		mountain->AddComponent<LODMeshHolder>().SetLODMesh(MeshBuilder::GetInstance().GenerateTerrain("Mountain Low", "Image//Heightmaps//Mountain//Low.raw", heightMap, 10, 10),
			MeshBuilder::GetInstance().GenerateTerrain("Mountain Mid", "Image//Heightmaps//Mountain//Mid.raw", heightMap, 10, 10),
			MeshBuilder::GetInstance().GenerateTerrain("Mountain High", "Image//Heightmaps//Mountain//High.raw", heightMap, 10, 10));
		mountain->GetComponent<LODMeshHolder>().textureList[LODMeshHolder::DETAIL_LEVEL::LOW_DETAILS].textureArray[0] = TextureManager::GetInstance().AddTexture("Mountain Low", "Image//Game//Environment//Rock//Low.tga");
		mountain->GetComponent<LODMeshHolder>().textureList[LODMeshHolder::DETAIL_LEVEL::MID_DETAILS].textureArray[0] = TextureManager::GetInstance().AddTexture("Mountain Mid", "Image//Game//Environment//Rock//Mid.tga");
		mountain->GetComponent<LODMeshHolder>().textureList[LODMeshHolder::DETAIL_LEVEL::HIGH_DETAILS].textureArray[0] = TextureManager::GetInstance().AddTexture("Mountain High", "Image//Game//Environment//Rock//High.tga");
	}

	{
		vector<unsigned char> heightMap;
		GameObject* mountain = &GameObjectFactory::CreateEmpty(name, "Mountain");
		mountain->GetComponent<Transform>().SetLocalPosition(0, -20, -500);
		mountain->GetComponent<Transform>().SetLocalScale(500, 70, 1000);
		mountain->GetComponent<Transform>().SetLocalRotation(0, 90, 0);
		mountain->AddComponent<LODMeshHolder>().SetLODMesh(MeshBuilder::GetInstance().GenerateTerrain("Mountain Low", "Image//Heightmaps//Mountain//Low.raw", heightMap, 10, 10),
			MeshBuilder::GetInstance().GenerateTerrain("Mountain Mid", "Image//Heightmaps//Mountain//Mid.raw", heightMap, 10, 10),
			MeshBuilder::GetInstance().GenerateTerrain("Mountain High", "Image//Heightmaps//Mountain//High.raw", heightMap, 10, 10));
		mountain->GetComponent<LODMeshHolder>().textureList[LODMeshHolder::DETAIL_LEVEL::LOW_DETAILS].textureArray[0] = TextureManager::GetInstance().AddTexture("Mountain Low", "Image//Game//Environment//Rock//Low.tga");
		mountain->GetComponent<LODMeshHolder>().textureList[LODMeshHolder::DETAIL_LEVEL::MID_DETAILS].textureArray[0] = TextureManager::GetInstance().AddTexture("Mountain Mid", "Image//Game//Environment//Rock//Mid.tga");
		mountain->GetComponent<LODMeshHolder>().textureList[LODMeshHolder::DETAIL_LEVEL::HIGH_DETAILS].textureArray[0] = TextureManager::GetInstance().AddTexture("Mountain High", "Image//Game//Environment//Rock//High.tga");
	}

	{
		//Create Rifle
		Mesh* rifleMeshLowLOD = MeshBuilder::GetInstance().GenerateOBJ("M24R Mesh Low LOD", "OBJ//Game//M24R//M24RLow.obj");
		Mesh* rifleMeshMidLOD = MeshBuilder::GetInstance().GenerateOBJ("M24R Mesh Mid LOD", "OBJ//Game//M24R//M24RMid.obj");
		Mesh* rifleMeshHighLOD = MeshBuilder::GetInstance().GenerateOBJ("M24R Mesh High LOD", "OBJ//Game//M24R//M24RHigh.obj");
		GameObject* playerRifle = &GameObjectFactory::CreateEquippableRifle(name);
		playerRifle->GetComponent<Transform>().SetLocalPosition(25, 1, 25);
		playerRifle->GetComponent<LODMeshHolder>().SetLODMesh(rifleMeshLowLOD, rifleMeshMidLOD, rifleMeshHighLOD);
		playerRifle->GetComponent<LODMeshHolder>().textureList[LODMeshHolder::DETAIL_LEVEL::HIGH_DETAILS].textureArray[0] = TextureManager::GetInstance().AddTexture("Rifle Texture Low", "Image//Game//M24R//M24R.tga");
		playerRifle->GetComponent<LODMeshHolder>().textureList[LODMeshHolder::DETAIL_LEVEL::MID_DETAILS].textureArray[0] = TextureManager::GetInstance().AddTexture("Rifle Texture Mid", "Image//Game/M24R//M24R.tga");
		playerRifle->GetComponent<LODMeshHolder>().textureList[LODMeshHolder::DETAIL_LEVEL::LOW_DETAILS].textureArray[0] = TextureManager::GetInstance().AddTexture("Rifle Texture High", "Image//Game//M24R//M24R.tga");
	}


	{
		Mesh* enemyMeshLowLOD = MeshBuilder::GetInstance().GenerateCube(" Enemy Mesh Low LOD", Color(1, 1, 0));
		Mesh* enemyMeshMidLOD = MeshBuilder::GetInstance().GenerateCube(" Enemy Mesh Mid LOD", Color(0, 1, 1));
		Mesh* enemyMeshHighLOD = MeshBuilder::GetInstance().GenerateCube("Enemy Mesh High LOD", Color(1, 1, 1));
		GameObject* enemy = &GameObjectFactory::CreateDefaultMovingEnemy(name);
		enemy->GetComponent<Transform>().SetLocalPosition(50 , 1, 50 );
		enemy->GetComponent<LODMeshHolder>().SetLODMesh(enemyMeshLowLOD, enemyMeshMidLOD, enemyMeshHighLOD);

		//for (int i = -6; i <= 6; i++)
		//{
		//	if (i > -2 && i < 2)
		//		continue;
		//	//Create Enemy Here
		//	Mesh* enemyMeshLowLOD = MeshBuilder::GetInstance().GenerateCube(" Enemy Mesh Low LOD", Color(1, 1, 0));
		//	Mesh* enemyMeshMidLOD = MeshBuilder::GetInstance().GenerateCube(" Enemy Mesh Mid LOD", Color(0, 1, 1));
		//	Mesh* enemyMeshHighLOD = MeshBuilder::GetInstance().GenerateCube("Enemy Mesh High LOD", Color(1, 1, 1));
		//	GameObject* enemy = &GameObjectFactory::CreateDefaultMovingEnemy(name);
		//	enemy->GetComponent<Transform>().SetLocalPosition(50 * i, 1 , 50 * i);
		//	enemy->GetComponent<LODMeshHolder>().SetLODMesh(enemyMeshLowLOD, enemyMeshMidLOD, enemyMeshHighLOD);
		//}
		/*
		for (int i = -6; i <= 6; i++)
		{
			if (i > -2 && i < 2)
				continue;
			Mesh* stationaryEnemyMeshLowLOD = MeshBuilder::GetInstance().GenerateCube(" Enemy Mesh Low LOD", Color(1, 1, 0));
			Mesh* stationaryEnemyMeshMidLOD = MeshBuilder::GetInstance().GenerateCube(" Enemy Mesh Mid LOD", Color(0, 1, 1));
			Mesh* stationaryEnemyMeshHighLOD = MeshBuilder::GetInstance().GenerateCube("Enemy Mesh High LOD", Color(1, 1, 1));
			GameObject* enemy2 = &GameObjectFactory::CreateDefaultEnemy(name);
			enemy2->GetComponent<Transform>().SetLocalPosition(30 * i, 1, 30 * i);
			enemy2->GetComponent<LODMeshHolder>().SetLODMesh(stationaryEnemyMeshLowLOD, stationaryEnemyMeshMidLOD, stationaryEnemyMeshHighLOD);
		}*/
	}

	{
		//Create Rifle
		Mesh* rifleMeshLowLOD = MeshBuilder::GetInstance().GenerateOBJ("M4A1 Mesh Low LOD", "OBJ//Game//M4A1//M4A1Low.obj");
		Mesh* rifleMeshMidLOD = MeshBuilder::GetInstance().GenerateOBJ("M4A1 Mesh Mid LOD", "OBJ//Game//M4A1//M4A1Mid.obj");
		Mesh* rifleMeshHighLOD = MeshBuilder::GetInstance().GenerateOBJ("M4A1 Mesh High LOD", "OBJ//Game//M4A1//M4A1High.obj");
		GameObject* playerRifle = &GameObjectFactory::CreateEquippableRifle(name);
		playerRifle->GetComponent<Transform>().SetLocalPosition(-25, 1, 25);
		playerRifle->GetComponent<LODMeshHolder>().SetLODMesh(rifleMeshLowLOD, rifleMeshMidLOD, rifleMeshHighLOD);
		playerRifle->GetComponent<LODMeshHolder>().textureList[LODMeshHolder::DETAIL_LEVEL::HIGH_DETAILS].textureArray[0] = TextureManager::GetInstance().AddTexture("Rifle Texture Low", "Image//Game//M4A1//M4A1.tga");
		playerRifle->GetComponent<LODMeshHolder>().textureList[LODMeshHolder::DETAIL_LEVEL::MID_DETAILS].textureArray[0] = TextureManager::GetInstance().AddTexture("Rifle Texture Mid", "Image//Game/M4A1//M4A1.tga");
		playerRifle->GetComponent<LODMeshHolder>().textureList[LODMeshHolder::DETAIL_LEVEL::LOW_DETAILS].textureArray[0] = TextureManager::GetInstance().AddTexture("Rifle Texture High", "Image//Game//M4A1//M4A1.tga");
	}

	{
		GameObject& core = GameObjectFactory::CreateEmpty(name, "Core");
		core.AddComponent<LODMeshHolder>().SetLODMesh(MeshBuilder::GetInstance().GenerateOBJ("Core Low", "OBJ//Game//Core//Low.obj"),
													  MeshBuilder::GetInstance().GenerateOBJ("Core Mid", "OBJ//Game//Core//Mid.obj"),
													  MeshBuilder::GetInstance().GenerateOBJ("Core High", "OBJ//Game//Core//High.obj"));
		core.GetComponent<LODMeshHolder>().textureList[LODMeshHolder::DETAIL_LEVEL::HIGH_DETAILS].textureArray[0] = TextureManager::GetInstance().AddTexture("Core Texture Low", "Image//Game//Core//Core.tga");
		core.GetComponent<LODMeshHolder>().textureList[LODMeshHolder::DETAIL_LEVEL::MID_DETAILS].textureArray[0] = TextureManager::GetInstance().AddTexture("Core Texture Mid", "Image//Game/Core//Core.tga");
		core.GetComponent<LODMeshHolder>().textureList[LODMeshHolder::DETAIL_LEVEL::LOW_DETAILS].textureArray[0] = TextureManager::GetInstance().AddTexture("Core Texture High", "Image//Game//Core//Core.tga");
		core.GetComponent<MeshRenderer>().lightEnabled = false;

		core.AddComponent<HealthComponent>();
		core.CreateScript<HealthScript>();
		core.AddComponent<ColliderGroup<SphereCollider>>().colliders[0].SetRadius(7.5);

		core.GetComponent<Transform>().SetLocalPosition(0, 25, -70);
		core.GetComponent<Transform>().SetLocalScale(15, 15, 15);
		core.CreateScript<RotateScript>();

		GameObject& ring = GameObjectFactory::CreateEmpty(name, "Core Ring");
		ring.AddComponent<LODMeshHolder>().SetLODMesh(MeshBuilder::GetInstance().GenerateOBJ("Core Ring Low", "OBJ//Game//Core//Ring//Low.obj"),
			MeshBuilder::GetInstance().GenerateOBJ("Core Ring Mid", "OBJ//Game//Core//Ring//Mid.obj"),
			MeshBuilder::GetInstance().GenerateOBJ("Core Ring High", "OBJ//Game//Core//Ring//High.obj"));
		//ring.GetComponent<LODMeshHolder>().textureList[LODMeshHolder::DETAIL_LEVEL::HIGH_DETAILS].textureArray[0] = TextureManager::GetInstance().AddTexture("Core Ring Texture Low","Image//Game//Core//Core.tga");
		//ring.GetComponent<LODMeshHolder>().textureList[LODMeshHolder::DETAIL_LEVEL::MID_DETAILS].textureArray[0] = TextureManager::GetInstance().AddTexture("Core Ring Texture Mid", "Image//Game/Core//Core.tga");
		//ring.GetComponent<LODMeshHolder>().textureList[LODMeshHolder::DETAIL_LEVEL::LOW_DETAILS].textureArray[0] = TextureManager::GetInstance().AddTexture("Core Ring Texture High", "Image//Game//Core//Core.tga");
		ring.SetParent(core);
		ring.CreateScript<RotateScript>();

		GameObject& ring2 = GameObjectFactory::CreateEmpty(name, "Core Ring");
		ring2.AddComponent<LODMeshHolder>().SetLODMesh(MeshBuilder::GetInstance().GenerateOBJ("Core Ring Low", "OBJ//Game//Core//Ring//Low.obj"),
			MeshBuilder::GetInstance().GenerateOBJ("Core Ring Mid", "OBJ//Game//Core//Ring//Mid.obj"),
			MeshBuilder::GetInstance().GenerateOBJ("Core Ring High", "OBJ//Game//Core//Ring//High.obj"));
		//ring2.GetComponent<LODMeshHolder>().textureList[LODMeshHolder::DETAIL_LEVEL::HIGH_DETAILS].textureArray[0] = TextureManager::GetInstance().AddTexture("Core Ring Texture Low","Image//Game//Core//Core.tga");
		//ring2.GetComponent<LODMeshHolder>().textureList[LODMeshHolder::DETAIL_LEVEL::MID_DETAILS].textureArray[0] = TextureManager::GetInstance().AddTexture("Core Ring Texture Mid", "Image//Game/Core//Core.tga");
		//ring2.GetComponent<LODMeshHolder>().textureList[LODMeshHolder::DETAIL_LEVEL::LOW_DETAILS].textureArray[0] = TextureManager::GetInstance().AddTexture("Core Ring Texture High", "Image//Game//Core//Core.tga");
		ring2.SetParent(core);
		ring2.GetComponent<Transform>().SetLocalScale(1.2, 1.2, 1.2);
		ring2.GetComponent<Transform>().SetLocalRotation(90, 90, 90);
		ring2.CreateScript<RotateScript>();
	}

	//{
		//for (int i = 0; i < 3; ++i)
		//{
		//	//Cube
		//	GameObject* cube = &GameObjectFactory::CreateCube(name, "Enemy");
		//	cube->GetComponent<Transform>().SetLocalPosition(10 * i, 5, 0);
		//	cube->GetComponent<MeshHolder>().textureList.textureArray[0] = TextureManager::GetInstance().AddTexture("Test Cube", "Image//Default//Test_Cube.tga");
		//	cube->AddComponent<ColliderGroup<SphereCollider>>().colliders[0].SetRadius(1.0f);
		//	cube->AddComponent<HealthComponent>();
		//	cube->CreateScript<HealthScript>();			
		//	cube->CreateScript<TranslateScript>();

		//	//Cube 2
		//	GameObject* cube2 = &GameObjectFactory::CreateCube(name, "Enemy");
		//	cube2->SetParent(*cube);
		//	cube2->GetComponent<Transform>().SetLocalPosition(0, 2, 1);
		//	cube2->AddComponent<HealthComponent>();
		//	cube2->CreateScript<HealthScript>();
		//	cube2->AddComponent<ColliderGroup<SphereCollider>>().colliders[0].SetRadius(1.0f);
		//	cube2->CreateScript<RotateScript>();

		//	//Cylinder
		//	GameObject* cylinder = &GameObjectFactory::CreateCylinder(name, "Enemy");
		//	cylinder->SetParent(*cube2);
		//	cylinder->GetComponent<Transform>().SetLocalPosition(2, 2, 2);
		//	cylinder->AddComponent<HealthComponent>();
		//	cylinder->CreateScript<HealthScript>();
		//	cylinder->AddComponent<ColliderGroup<SphereCollider>>().colliders[0].SetRadius(1.0f);
		//}
    //}

}

void SceneAsn1::Update(double _deltaTime) {
	SpatialPartitionSystem::GetInstance().Update(name);

	PhysicsSystem::GetInstance().UpdateDeltaTime(name, _deltaTime);
	GameObjectManager::GetInstance().UpdateScripts(name, _deltaTime);		
	PhysicsSystem::GetInstance().Update(name);

	//Close da app
	if (InputManager::GetInstance().GetInputInfo().keyDown[INPUT_QUIT]) {
		Application::GetInstance().Quit();
	}
 
	RenderSystem::GetInstance().Update(name, _deltaTime);
}

void SceneAsn1::Render() {
	SpatialPartitionSystem::GetInstance().Update(name);

	GraphicsManager::GetInstance().Enable<GraphicsManager::MODE::DEPTH_TEST>();	
	RenderSystem::GetInstance().Render(name, &skybox);
}

void SceneAsn1::Exit() {	
	RenderHelper::GetInstance().DeleteShader(SHADERS::PHONG);
	GameObjectManager::GetInstance().Clear(name);
}
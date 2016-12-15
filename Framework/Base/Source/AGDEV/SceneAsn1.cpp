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
	int xGridSize = 25; int yGridSize = 50; int zGridSize = 25;
	int xNumGrid = 50; int yNumGrid = 5; int zNumGrid = 50;
	SpatialPartitionSystem::GetInstance().CreateSpatialPartition(name)->Set(xGridSize, yGridSize, zGridSize, xNumGrid, yNumGrid, zNumGrid, 0, ((yNumGrid >> 1) - 1) * yGridSize, 0);

	//Player
	player = &GameObjectFactory::CreateEmpty(name, "Player");
	player->CreateScript<PlayerMovementScript>();
	player->CreateScript<DebugControlsScript>();
	player->CreateScript<HealthScript>();
	player->AddComponent<HealthComponent>();
	player->CreateScript<PlayerActionScript>();
	player->AddComponent<Rigidbody>().elasticity = 0.0f;
	player->AddComponent<ColliderGroup<SphereCollider>>().CreateColliders(1);
	player->GetComponent<ColliderGroup<SphereCollider>>().colliders[0].SetRadius(5.f);
	player->GetComponent<ColliderGroup<SphereCollider>>().colliders[1].SetRadius(2.f);
	player->GetComponent<ColliderGroup<SphereCollider>>().colliders[1].isTrigger = true;
	player->AddComponent<ColliderGroup<AABBCollider>>().colliders[0];
	//player->AddComponent<MeshHolder>().mesh = MeshBuilder::GetInstance().GenerateOBJ("Player Sphere", "OBJ//Default//Sphere.obj");
 
	//Camera
	GameObject* camera = &GameObjectFactory::CreateCamera(name,"Player Camera");
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
	ground->GetComponent<Transform>().IgnoreSpatialPartition(true);
	ground->GetComponent<MeshHolder>().textureList.textureArray[0] = TextureManager::GetInstance().AddTexture("Ground Grass", "Image//Game//Environment//Grass.tga");
	ground->AddComponent<ColliderGroup<AABBCollider>>().colliders[0].centre.Set(0, -250.0f, 0);
	ground->GetComponent<ColliderGroup<AABBCollider>>().colliders[0].size.Set(1000, 500, 1000);
	ground->GetComponent<ColliderGroup<AABBCollider>>().colliders[0].isGround = true;
	
	//Background Mountain
	{
		vector<unsigned char> heightMap;
		GameObject* mountain = &GameObjectFactory::CreateEmpty(name, "Mountain");
		mountain->GetComponent<Transform>().SetLocalPosition(0, -20, 500);
		mountain->GetComponent<Transform>().SetLocalScale(1000, 70 , 500);
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
		mountain->GetComponent<Transform>().SetLocalScale(500, 70 , 1000);
		mountain->GetComponent<Transform>().SetLocalRotation(0, 90 , 0);
		mountain->AddComponent<LODMeshHolder>().SetLODMesh(MeshBuilder::GetInstance().GenerateTerrain("Mountain Low", "Image//Heightmaps//Mountain//Low.raw", heightMap, 10, 10),
														   MeshBuilder::GetInstance().GenerateTerrain("Mountain Mid", "Image//Heightmaps//Mountain//Mid.raw", heightMap, 10, 10),
														   MeshBuilder::GetInstance().GenerateTerrain("Mountain High", "Image//Heightmaps//Mountain//High.raw", heightMap, 10, 10));
		mountain->GetComponent<LODMeshHolder>().textureList[LODMeshHolder::DETAIL_LEVEL::LOW_DETAILS].textureArray[0] = TextureManager::GetInstance().AddTexture("Mountain Low", "Image//Game//Environment//Rock//Low.tga");
		mountain->GetComponent<LODMeshHolder>().textureList[LODMeshHolder::DETAIL_LEVEL::MID_DETAILS].textureArray[0] = TextureManager::GetInstance().AddTexture("Mountain Mid", "Image//Game//Environment//Rock//Mid.tga");
		mountain->GetComponent<LODMeshHolder>().textureList[LODMeshHolder::DETAIL_LEVEL::HIGH_DETAILS].textureArray[0] = TextureManager::GetInstance().AddTexture("Mountain High", "Image//Game//Environment//Rock//High.tga");
	}

	for (unsigned int g = 0; g < 400; ++g) {
		Vector3 spawnPos(Math::RandFloatMinMax(-300.0f, 300.0f), 0, Math::RandFloatMinMax(-300.0f, 300.0f));
		GameObject& grass = GameObjectFactory::CreateGrass(name, "Grass");
		grass.GetComponent<Transform>().SetLocalPosition(spawnPos);
		grass.GetComponent<Transform>().SetLocalScale(4, 4, 4);
		grass.GetComponent<Transform>().SetLocalRotation(0, Math::RandFloatMinMax(0.0f, 360.0f), 0);
	}	

	{
		//Create Cutscene Trigger
		GameObject* cutsceneTrigger = &GameObjectFactory::CreateCutsceneTrigger(name);
		cutsceneTrigger->GetComponent<Transform>().SetLocalPosition(25, 0, 25);

		//Create Rifle
		//Mesh* rifleMeshLowLOD = MeshBuilder::GetInstance().GenerateOBJ("Rifle Mesh Low LOD","OBJ//Game//M24R//M24RLow.obj");
		//Mesh* rifleMeshMidLOD = MeshBuilder::GetInstance().GenerateOBJ(" Rifle Mesh Mid LOD", "OBJ//Game//M24R//M24RMid.obj");
		//Mesh* rifleMeshHighLOD = MeshBuilder::GetInstance().GenerateOBJ("Rifle Mesh High LOD", "OBJ//Game//M24R//M24RHigh.obj");
		Mesh* rifleMeshLowLOD = MeshBuilder::GetInstance().GenerateOBJ("Rifle Mesh Low LOD","OBJ//Default//Cube.obj");
		Mesh* rifleMeshMidLOD = MeshBuilder::GetInstance().GenerateOBJ(" Rifle Mesh Mid LOD", "OBJ//Default//Cube.obj");
		Mesh* rifleMeshHighLOD = MeshBuilder::GetInstance().GenerateOBJ("Rifle Mesh High LOD", "OBJ//Default//Cube.obj");
		GameObject* playerRifle = &GameObjectFactory::CreateEquippableRifle(name);
		playerRifle->GetComponent<Transform>().SetLocalPosition(25, 1, 25);
		playerRifle->GetComponent<LODMeshHolder>().SetLODMesh(rifleMeshLowLOD, rifleMeshMidLOD, rifleMeshHighLOD);
		playerRifle->GetComponent<LODMeshHolder>().textureList[LODMeshHolder::DETAIL_LEVEL::HIGH_DETAILS].textureArray[0] = TextureManager::GetInstance().AddTexture("Rifle Texture Low","Image//Game//M24R//M24R.tga");
		playerRifle->GetComponent<LODMeshHolder>().textureList[LODMeshHolder::DETAIL_LEVEL::MID_DETAILS].textureArray[0] = TextureManager::GetInstance().AddTexture("Rifle Texture Mid", "Image//Game/M24R//M24R.tga");
		playerRifle->GetComponent<LODMeshHolder>().textureList[LODMeshHolder::DETAIL_LEVEL::LOW_DETAILS].textureArray[0] = TextureManager::GetInstance().AddTexture("Rifle Texture High", "Image//Game//M24R//M24R.tga");
	}	

	{
		for (unsigned int i = 0; i < 6; ++ i) {
			GameObject& platform = GameObjectFactory::CreatePlatform(name);
			platform.GetComponent<Transform>().SetLocalPosition(20, i * 2 + 1, 20 + (i * 8));
		}
	}

}

void SceneAsn1::Update(double _deltaTime) {	
	PhysicsSystem::GetInstance().UpdateDeltaTime(name, _deltaTime);

	GameObjectManager::GetInstance().UpdateScripts(name, _deltaTime);
	SpatialPartitionSystem::GetInstance().Update(name);
		
	PhysicsSystem::GetInstance().Update(name);

	//Close da app
	if (InputManager::GetInstance().GetInputInfo().keyDown[INPUT_QUIT]) {
		Application::GetInstance().Quit();
	}
 
	RenderSystem::GetInstance().Update(name, _deltaTime);
	debugCountdown -= _deltaTime;

	if (debugCountdown <= 0)
	{
		cout << "FPS: " << 1.0 / _deltaTime << endl;
	} 
}

void SceneAsn1::Render() {
	GraphicsManager::GetInstance().Enable<GraphicsManager::MODE::DEPTH_TEST>();
	RenderSystem::GetInstance().Render(name, &skybox);

	if (debugCountdown <= 0)
	{
		//SpatialPartitionSystem::GetInstance().GetSpatialPartition(name)->PrintSelf();
		debugCountdown = 2.f;
	}
}

void SceneAsn1::Exit() {	
	RenderHelper::GetInstance().DeleteShader(SHADERS::PHONG);
	GameObjectManager::GetInstance().Clear(name);
}
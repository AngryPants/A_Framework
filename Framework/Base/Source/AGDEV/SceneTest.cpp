#include "SceneTest.h"
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
#include "../Scene/SceneManager.h"
//Include Scripts
#include "RotateScript.h"
#include "ScaleScript.h"
#include "PlayerCameraScript.h"
#include "PlayerMovementScript.h"
#include "TranslateScript.h"
#include "PivotScript.h"
#include "DebugControlsScript.h" 
#include "TriggerTestScript.h" 
#include "../Script/PlayerShootingScript.h"
#include "../Script/PlayerActionScript.h"
#include "../Script/PlayerHealthScript.h"

//Constructor(s) & Destructor
SceneTest::SceneTest(const string& _name) : Scene(_name) {
}

SceneTest::~SceneTest() {
}

//Interface Function(s)
void SceneTest::Init() {
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
	int xGridSize = 10; int yGridSize = 10; int zGridSize = 10;
	int xNumGrid = 13; int yNumGrid = 7; int zNumGrid = 13;  
	SpatialPartitionSystem::GetInstance().CreateSpatialPartition(name)->Set(xGridSize, yGridSize, zGridSize, xNumGrid, yNumGrid, zNumGrid, 0, ((yNumGrid >> 1) - 1) * yGridSize, 0);
	//SpatialPartitionSystem::GetInstance().CreateSpatialPartition(name)->Set(xGridSize, yGridSize, zGridSize, xNumGrid, yNumGrid, zNumGrid);

	//Player
	player = &GameObjectFactory::CreateEmpty(name, "Player");
	player->CreateScript<PlayerMovementScript>();
	player->CreateScript<PlayerActionScript>();
	player->AddComponent<HealthComponent>();
	player->CreateScript<PlayerHealthScript>();
	player->AddComponent<Rigidbody>().elasticity = 0.0f;
	player->AddComponent<ColliderGroup<SphereCollider>>().CreateColliders(1);
	player->GetComponent<ColliderGroup<SphereCollider>>().colliders[0].SetRadius(1.f);
	player->GetComponent<ColliderGroup<SphereCollider>>().colliders[1].SetRadius(2.f);
	player->GetComponent<ColliderGroup<SphereCollider>>().colliders[1].isTrigger = true;
	player->AddComponent<ColliderGroup<AABBCollider>>().colliders[0];
	//player->AddComponent<MeshHolder>().mesh = MeshBuilder::GetInstance().GenerateOBJ("Player Sphere", "OBJ//Default//Sphere.obj");
 
	//Camera
	GameObject* camera = &GameObjectFactory::CreateCamera(name,"Player Camera");
	camera->SetParent(*player);
	camera->GetComponent<Transform>().SetLocalPosition(0, 1.7f, 0);
	camera->CreateScript<PlayerCameraScript>();
	camera->CreateScript<DebugControlsScript>();
	camera->AddComponent<TextRenderer>().mesh = MeshBuilder::GetInstance().GenerateText("Text", 16, 16);
	camera->AddComponent<TextRenderer>().textureList.textureArray[0] = TextureManager::GetInstance().AddTexture("Text", "Image//Fonts//Consolas.tga");
	camera->AddComponent<TextRenderer>().text = "HELLO WORLD";

	//Lights
	GameObject* light = &GameObjectFactory::CreateLight(name);
	light->GetComponent<Light>().type = Light::LIGHT_TYPE::LIGHT_DIRECTIONAL;
	light->GetComponent<Light>().power = 5.0f;
	light->GetComponent<Transform>().SetLocalPosition(0, 0, 5);
	light->GetComponent<Transform>().SetLocalRotation(45, 0, 0);

	////Cube
	//GameObject* cube = &GameObjectFactory::CreateCube(name, "Cube 1");
	//cube->GetComponent<Transform>().SetLocalPosition(5, 1, 0);
	//cube->GetComponent<MeshHolder>().textureList.textureArray[0] = TextureManager::GetInstance().AddTexture("Test Cube", "Image//Default//Test_Cube.tga");
	//cube->CreateScript<RotateScript>();
	//cube->CreateScript<TranslateScript>();

	////Cube 2
	//GameObject* cube2 = &GameObjectFactory::CreateCube(name, "Cube 2");
	//cube2->SetParent(*cube);
	//cube2->GetComponent<Transform>().SetLocalPosition(0, 0, -2);
	//cube2->CreateScript<ScaleScript>();
	//cube2->CreateScript<RotateScript>();

	////Cylinder
	//GameObject* cylinder = &GameObjectFactory::CreateCylinder(name, "Cylinder");
	//cylinder->SetParent(*cube2);
	//cylinder->GetComponent<Transform>().SetLocalPosition(2, -2, 4);
	//cylinder->CreateScript<TranslateScript>();
	//cylinder->CreateScript<RotateScript>();

	//Ground 
	//GameObject* ground = &GameObjectFactory::CreatePlane(name, "Ground");
	//ground->GetComponent<Transform>().SetLocalPosition(0, 0, 0);
	//ground->GetComponent<Transform>().SetLocalScale(100, 100 ,100);
	//ground->GetComponent<Transform>().IgnoreSpatialPartition(true);
	//ground->GetComponent<MeshHolder>().textureList.textureArray[0] = TextureManager::GetInstance().AddTexture("Test Texture", "Image//Default//Test_Texture.tga");
	 
	GameObject* ground = &GameObjectFactory::CreatePlane(name, "Ground");
	ground->GetComponent<Transform>().SetLocalPosition(0, 0, 0);
	ground->GetComponent<Transform>().SetLocalScale(100, 100 ,100);
	ground->useSpatialPartition = false;
	ground->GetComponent<MeshHolder>().textureList.textureArray[0] = TextureManager::GetInstance().AddTexture("Test Texture", "Image//Default//Test_Texture.tga");
	ground->AddComponent<ColliderGroup<AABBCollider>>().colliders[0].centre.Set(0, -250.0f, 0);
	ground->GetComponent<ColliderGroup<AABBCollider>>().colliders[0].size.Set(100, 500, 100);
	ground->GetComponent<ColliderGroup<AABBCollider>>().colliders[0].isGround = true;
	//ground->GetComponent<MeshRenderer>().SetInActive();

	//Test Box
	//GameObject* testBox = &GameObjectFactory::CreateCube(name, "Test Box");
	//testBox->GetComponent<Transform>().SetLocalPosition(0, 100, 5);
	//testBox->AddComponent<Rigidbody>().elasticity = 0.1f;
	//testBox->AddComponent<ColliderGroup<AABBCollider>>().colliders[0].size.Set(1, 1, 1);

	//GameObject* testBox2 = &GameObjectFactory::CreateCube(name, "Test Box 2");
	//testBox2->GetComponent<Transform>().SetLocalPosition(1, 1.1, 5);
	//testBox2->AddComponent<Rigidbody>().elasticity = 0.1f;
	//testBox2->AddComponent<ColliderGroup<AABBCollider>>().colliders[0].size.Set(1, 1, 1);

	////Platforms
	//for (int p = 0; p < 4; ++p) {
	//	GameObject* platform = &GameObjectFactory::CreateCube(name, "Platform");
	//	platform->GetComponent<Transform>().SetLocalScale(10, 5, 10);
	//	platform->GetComponent<Transform>().SetLocalPosition(5, 2.5 * p, p * 10);
	//	platform->AddComponent<ColliderGroup<AABBCollider>>().colliders[0].size.Set(10, 5, 10);
	//	platform->GetComponent<ColliderGroup<AABBCollider>>().colliders[0].isGround = true;
	//}

	//LOD Sphere
	//Mesh* meshLowLOD = MeshBuilder::GetInstance().GenerateSphere("Mesh Low LOD", Color(0, 1, 1), 6, 6, 0.5f);
	//Mesh* meshMidLOD = MeshBuilder::GetInstance().GenerateSphere("Mesh Mid LOD", Color(0, 1, 0), 8, 8, 0.5f);
	//Mesh* meshHighLOD = MeshBuilder::GetInstance().GenerateSphere("Mesh High LOD", Color(1, 0, 0), 32, 32, 0.5f);
	//GameObject* sphereLOD = &GameObjectFactory::CreateEmpty(name, "Sphere LOD");
	//sphereLOD->AddComponent<LODMeshHolder>().SetLODMesh(meshLowLOD, meshMidLOD, meshHighLOD);
	//sphereLOD->GetComponent<MeshRenderer>().lightEnabled = true;
	//sphereLOD->GetComponent<Transform>().SetLocalScale(5, 5, 5);
	//sphereLOD->GetComponent<Transform>().SetLocalPosition(SpatialPartitionSystem::GetInstance().GetSpatialPartition(name)->GetGrid(5, 2, 8).GetPosition());

	//Physics Balls
	//for (unsigned int n = 0; n < 5; ++n) {
	//	GameObject& ball = GameObjectFactory::CreateSphere(name, "Physics Ball");
	//	ball.GetComponent<Transform>().SetLocalPosition(0 + (n % 2) * 2.0f, n + 0.1f, 20.1);
	//	//ball.GetComponent<Transform>().SetLocalPosition(Math::RandFloatMinMax(-100, 100), Math::RandFloatMinMax(0, 100), Math::RandFloatMinMax(-100, 100));
	//	//ball.GetComponent<Transform>().IgnoreSpatialPartition(true);
	//	ball.AddComponent<Rigidbody>().velocity.Set(10, 0, 0);
	//	ball.AddComponent<Rigidbody>().useGravity = false;
	//	ball.AddComponent<ColliderGroup<SphereCollider>>().colliders[0].isTrigger = false;
	//	ball.CreateScript<TriggerTestScript>();
	//}
	//
	//for (unsigned int n = 0; n < 5; ++n) {
	//	GameObject& ball2 = GameObjectFactory::CreateSphere(name, "Physics Ball");
	//	ball2.GetComponent<Transform>().SetLocalPosition(20 + (n % 2) * 2.0f, n, 20);
	//	//ball2.GetComponent<Transform>().SetLocalPosition(Math::RandFloatMinMax(-100, 100), Math::RandFloatMinMax(0, 100), Math::RandFloatMinMax(-100, 100));
	//	//ball2.GetComponent<Transform>().IgnoreSpatialPartition(true);
	//	//ball2.AddComponent<Rigidbody>().velocity.Set(-1, 0, 0);
	//	//ball2.AddComponent<Rigidbody>().useGravity = false;
	//	ball2.AddComponent<ColliderGroup<SphereCollider>>().colliders[0].isTrigger = false;
	//	ball2.CreateScript<TriggerTestScript>();
	//}

	//SkyBox
	//GameObject* skyBox = &GameObjectFactory::CreateSkyBox(name);
	//skyBox->SetParent(*camera);
	//skyBox->GetComponent<Transform>().SetLocalScale(100, 100, 100);

	Mesh* meshLowLOD = MeshBuilder::GetInstance().GenerateSphere("Mesh Low LOD", Color(0, 1, 1), 6, 6, 0.5f);
	Mesh* meshMidLOD = MeshBuilder::GetInstance().GenerateSphere("Mesh Mid LOD", Color(0, 1, 0), 8, 8, 0.5f);
	Mesh* meshHighLOD = MeshBuilder::GetInstance().GenerateSphere("Mesh High LOD", Color(1, 0, 0), 32, 32, 0.5f);
	GameObject* sphereLOD = &GameObjectFactory::CreateEmpty(name, "Sphere LOD");
	sphereLOD->AddComponent<LODMeshHolder>().SetLODMesh(meshLowLOD, meshMidLOD, meshHighLOD);
	sphereLOD->GetComponent<MeshRenderer>().lightEnabled = true;
	sphereLOD->GetComponent<Transform>().SetLocalScale(5, 5, 5);
	sphereLOD->GetComponent<Transform>().SetLocalPosition(SpatialPartitionSystem::GetInstance().GetSpatialPartition(name)->GetGrid(5, 2, 8).GetPosition());
		
	//Create Enemy Here
	Mesh* enemyMeshLowLOD = MeshBuilder::GetInstance().GenerateCube(" Enemy Mesh Low LOD", Color(1, 1, 0));
	Mesh* enemyMeshMidLOD = MeshBuilder::GetInstance().GenerateCube(" Enemy Mesh Mid LOD", Color(0, 1, 1));
	Mesh* enemyMeshHighLOD = MeshBuilder::GetInstance().GenerateCube("Enemy Mesh High LOD", Color(1, 1, 1));
	GameObject* enemy = &GameObjectFactory::CreateDefaultMovingEnemy(name);
	enemy->GetComponent<Transform>().SetLocalPosition(5 , 1, 5);
	enemy->GetComponent<LODMeshHolder>().SetLODMesh(enemyMeshLowLOD,enemyMeshMidLOD,enemyMeshHighLOD);

	Mesh* stationaryEnemyMeshLowLOD = MeshBuilder::GetInstance().GenerateCube(" Enemy Mesh Low LOD", Color(1, 1, 0));
	Mesh* stationaryEnemyMeshMidLOD = MeshBuilder::GetInstance().GenerateCube(" Enemy Mesh Mid LOD", Color(0, 1, 1));
	Mesh* stationaryEnemyMeshHighLOD = MeshBuilder::GetInstance().GenerateCube("Enemy Mesh High LOD", Color(1, 1, 1));
	GameObject* enemy2 = &GameObjectFactory::CreateDefaultEnemy(name);
	enemy2->GetComponent<Transform>().SetLocalPosition(30, 1, 30);
	enemy2->GetComponent<LODMeshHolder>().SetLODMesh(stationaryEnemyMeshLowLOD, stationaryEnemyMeshMidLOD, stationaryEnemyMeshHighLOD);

	//Create Rifle
	Mesh* rifleMeshLowLOD = MeshBuilder::GetInstance().GenerateOBJ("Rifle Mesh Low LOD","OBJ//Game//M24R//M24RLow.obj");
	Mesh* rifleMeshMidLOD = MeshBuilder::GetInstance().GenerateOBJ(" Rifle Mesh Mid LOD", "OBJ//Game//M24R//M24RMid.obj");
	Mesh* rifleMeshHighLOD = MeshBuilder::GetInstance().GenerateOBJ("Rifle Mesh High LOD", "OBJ//Game//M24R//M24RHigh.obj");

	GameObject* playerRifle = &GameObjectFactory::CreateEquippableRifle(name);
	playerRifle->GetComponent<Transform>().SetLocalPosition(1, 1, 10);
	playerRifle->GetComponent<LODMeshHolder>().SetLODMesh(rifleMeshLowLOD, rifleMeshMidLOD, rifleMeshHighLOD);
	playerRifle->GetComponent<LODMeshHolder>().textureList[LODMeshHolder::DETAIL_LEVEL::HIGH_DETAILS].textureArray[0] = TextureManager::GetInstance().AddTexture("Rifle Texture Low","Image//Game//M24R//M24R.tga");
	playerRifle->GetComponent<LODMeshHolder>().textureList[LODMeshHolder::DETAIL_LEVEL::MID_DETAILS].textureArray[0] = TextureManager::GetInstance().AddTexture("Rifle Texture Mid", "Image//Game/M24R//M24R.tga");
	playerRifle->GetComponent<LODMeshHolder>().textureList[LODMeshHolder::DETAIL_LEVEL::LOW_DETAILS].textureArray[0] = TextureManager::GetInstance().AddTexture("Rifle Texture High", "Image//Game//M24R//M24R.tga");
}

void SceneTest::Update(double _deltaTime) {	
	if (InputManager::GetInstance().GetInputInfo().keyDown[INPUT_LOOK_ZOOM])
	{
		SceneManager::GetInstance().SetActiveScene("Assignment 1 Scene");
	}

	PhysicsSystem::GetInstance().UpdateDeltaTime(name, _deltaTime);

	GameObjectManager::GetInstance().UpdateScripts(name, _deltaTime);
	SpatialPartitionSystem::GetInstance().Update(name);
		
	PhysicsSystem::GetInstance().Update(name);

	//Close da app
	/*if (InputManager::GetInstance().GetInputInfo().keyDown[INPUT_QUIT]) {
		Application::GetInstance().Quit();
	}*/
 
	RenderSystem::GetInstance().Update(name, _deltaTime);
	debugCountdown -= _deltaTime;

	if (debugCountdown <= 0)
	{
		cout << "FPS: " << 1.0 / _deltaTime << endl;
	} 
}

void SceneTest::Render() {
	GraphicsManager::GetInstance().Enable<GraphicsManager::MODE::DEPTH_TEST>();
	RenderSystem::GetInstance().Render(name, &skybox);

	if (debugCountdown <= 0)
	{
		//SpatialPartitionSystem::GetInstance().GetSpatialPartition(name)->PrintSelf();
		debugCountdown = 2.f;
	}
}

void SceneTest::Exit() {	
	RenderHelper::GetInstance().DeleteShader(SHADERS::PHONG);
	GameObjectManager::GetInstance().Clear(name);
}
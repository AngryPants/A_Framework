#include "SceneTest.h"
#include "../Graphics/GraphicsManager.h"
#include "../Graphics/RenderHelper.h"
#include "../Application/Application.h"
#include "../Input/InputManager.h"
#include "../Mesh/MeshBuilder.h"
#include "../Texture/TextureManager.h"
#include "../Systems/Rendering//RenderSystem.h"
#include "../Systems/SpatialPartition/SPSystem.h"
#include "../Systems/Physics/PhysicsSystem.h"

//Include Scripts
#include "RotateScript.h"
#include "ScaleScript.h"
#include "PlayerCameraScript.h"
#include "PlayerMovementScript.h"
#include "TranslateScript.h"
#include "PivotScript.h"
#include "DebugControlsScript.h"
#include "TriggerTestScript.h"

//Constructor(s) & Destructor
SceneTest::SceneTest(const string& name) : Scene(name) {
}

SceneTest::~SceneTest() {
}

//Interface Function(s)
void SceneTest::Init() {
	//Initialise some GraphicsManager stuff.
	GraphicsManager::GetInstance().Enable<GraphicsManager::MODE::BLENDING>();
	GraphicsManager::GetInstance().Enable<GraphicsManager::MODE::CULLING>();

	//Initialise the Shader.
	RenderHelper::GetInstance().LoadShader(SHADERS::PHONG);
	RenderHelper::GetInstance().EnableLight(true);
	RenderHelper::GetInstance().SetNumLights(8);
	RenderHelper::GetInstance().EnableFog(false);
	RenderHelper::GetInstance().SetAlphaDiscardValue(0.1f);
	
	//SpatialPartition
	int xGridSize = 10; int yGridSize = 10; int zGridSize = 10;
	int xNumGrid = 11; int yNumGrid = 7; int zNumGrid = 11;
	SpatialPartitionSystem::GetInstance().CreateSpatialPartition(name)->Set(xGridSize, yGridSize, zGridSize, xNumGrid, yNumGrid, zNumGrid, 0, ((yNumGrid >> 1) - 1) * yGridSize, 0);
	//SpatialPartitionSystem::GetInstance().CreateSpatialPartition(name)->Set(xGridSize, yGridSize, zGridSize, xNumGrid, yNumGrid, zNumGrid);

	//Player
	player = &GameObjectFactory::CreateEmpty(name, "Player");
	player->CreateScript<PlayerMovementScript>();
	player->CreateScript<DebugControlsScript>();
	player->GetComponent<Transform>().SetLocalPosition(0, 0, -5);
	player->AddComponent<MeshHolder>().mesh = MeshBuilder::GetInstance().GenerateOBJ("Player Sphere", "OBJ//Default//Sphere.obj");

	//Camera
	GameObject* camera = &GameObjectFactory::CreateCamera(name,"Player Camera");
	camera->SetParent(*player);
	camera->GetComponent<Transform>().SetLocalPosition(0, 1.7f, 0);
	camera->CreateScript<PlayerCameraScript>();
	camera->GetComponent<Camera>().SetFarClippingPlane(100);

	//Lights
	GameObject* light = &GameObjectFactory::CreateLight(name);
	light->GetComponent<Light>().type = Light::LIGHT_TYPE::LIGHT_DIRECTIONAL;
	light->GetComponent<Light>().power = 5.0f;
	light->GetComponent<Transform>().SetLocalPosition(0, 0, 5);
	light->GetComponent<Transform>().SetLocalRotation(45, 0, 0);

	//Cube
	GameObject* cube = &GameObjectFactory::CreateCube(name, "Cube 1");
	cube->GetComponent<Transform>().SetLocalPosition(5, 1, 0);
	cube->GetComponent<MeshHolder>().textureList.textureArray[0] = TextureManager::GetInstance().AddTexture("Test Cube", "Image//Default//Test_Cube.tga");
	cube->CreateScript<RotateScript>();
	cube->CreateScript<TranslateScript>();

	//Cube 2
	GameObject* cube2 = &GameObjectFactory::CreateCube(name, "Cube 2");
	cube2->SetParent(*cube);
	cube2->GetComponent<Transform>().SetLocalPosition(0, 0, -2);
	cube2->CreateScript<ScaleScript>();
	cube2->CreateScript<RotateScript>();

	//Cylinder
	GameObject* cylinder = &GameObjectFactory::CreateCylinder(name, "Cylinder");
	cylinder->SetParent(*cube2);
	cylinder->GetComponent<Transform>().SetLocalPosition(2, -2, 4);
	cylinder->CreateScript<TranslateScript>();
	cylinder->CreateScript<RotateScript>();

	//Ground
	//GameObject* ground = &GameObjectFactory::CreatePlane(name, "Ground");
	//ground->GetComponent<Transform>().SetLocalPosition(0, 0, 0);
	//ground->GetComponent<Transform>().SetLocalScale(100, 100 ,100);
	//ground->GetComponent<Transform>().IgnoreSpatialPartition(true);
	//ground->GetComponent<MeshHolder>().textureList.textureArray[0] = TextureManager::GetInstance().AddTexture("Test Texture", "Image//Default//Test_Texture.tga");

	//LOD Sphere
	Mesh* meshLowLOD = MeshBuilder::GetInstance().GenerateSphere("Mesh Low LOD", Color(0, 1, 1), 4, 4, 0.5f);
	Mesh* meshMidLOD = MeshBuilder::GetInstance().GenerateSphere("Mesh Mid LOD", Color(0, 1, 0), 8, 8, 0.5f);
	Mesh* meshHighLOD = MeshBuilder::GetInstance().GenerateSphere("Mesh High LOD", Color(1, 0, 0), 32, 32, 0.5f);
	GameObject* sphereLOD = &GameObjectFactory::CreateEmpty(name, "Sphere LOD");
	sphereLOD->AddComponent<LODMeshHolder>().SetLODMesh(meshLowLOD, meshMidLOD, meshHighLOD);
	sphereLOD->GetComponent<MeshRenderer>().lightEnabled = true;
	sphereLOD->GetComponent<Transform>().SetLocalScale(10, 10, 10);
	sphereLOD->GetComponent<Transform>().SetLocalPosition(SpatialPartitionSystem::GetInstance().GetSpatialPartition(name)->GetGrid(5, 1, 8).GetPosition());

	//Physics Balls
	for (unsigned int n = 0; n < 200; ++n) {
		GameObject& ball = GameObjectFactory::CreateSphere(name, "Physics Ball");
		ball.GetComponent<Transform>().SetLocalPosition(0, 20 + n * 1, 20);		
		ball.AddComponent<Rigidbody>().velocity.Set(0, 10, 0);
		//ball.GetComponent<Rigidbody>().useGravity = false;
		ball.AddComponent<ColliderGroup<SphereCollider>>().CreateColliders(1);
		//ball.GetComponent<ColliderGroup<SphereCollider>>().colliders[0].isTrigger = true;
		//ball.CreateScript<TriggerTestScript>();
	}
	
	for (unsigned int n = 0; n < 200; ++n) {
		GameObject& ball = GameObjectFactory::CreateSphere(name, "Physics Ball");
		ball.GetComponent<Transform>().SetLocalPosition(-20, 20 + n * 1, 20);
		ball.AddComponent<Rigidbody>().velocity.Set(0, -10, 0);
		//ball.GetComponent<Rigidbody>().useGravity = false;
		ball.AddComponent<ColliderGroup<SphereCollider>>().CreateColliders(1);
		//ball.GetComponent<ColliderGroup<SphereCollider>>().colliders[0].isTrigger = true;
		//ball.CreateScript<TriggerTestScript>();
	}

}

void SceneTest::Update(double _deltaTime) {	
	PhysicsSystem::GetInstance().UpdateDeltaTime(name, _deltaTime);

	GameObjectManager::GetInstance().UpdateScripts(name, _deltaTime);
	SpatialPartitionSystem::GetInstance().Update(name);
		
	PhysicsSystem::GetInstance().UpdateGravity(name);
	PhysicsSystem::GetInstance().UpdateCollision(name);
	PhysicsSystem::GetInstance().UpdateMovement(name);

	//Close da app
	if (InputManager::GetInstance().GetInputInfo().keyDown[INPUT_QUIT]) {
		Application::GetInstance().Quit();
	}
	RenderSystem::GetInstance().Update(name, _deltaTime);

	debugCountdown -= _deltaTime;
	
}

void SceneTest::Render() {
	GraphicsManager::GetInstance().ClearColor();
	GraphicsManager::GetInstance().Enable<GraphicsManager::MODE::DEPTH_TEST>();

	RenderSystem::GetInstance().Render(name);

	//Render Entities UI
	GraphicsManager::GetInstance().Disable<GraphicsManager::MODE::DEPTH_TEST>();
	GraphicsManager::GetInstance().SetToUI();

	RenderSystem::GetInstance().RenderUI(name);
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
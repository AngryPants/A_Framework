#include "SceneTest.h"
#include "../Graphics/GraphicsManager.h"
#include "../Graphics/RenderHelper.h"
#include "../Application/Application.h"
#include "../Input/InputManager.h"
#include "../Mesh/MeshBuilder.h"
#include "../Texture/TextureManager.h"
#include "../Systems/Rendering//RenderSystem.h"
#include "../Systems/SpatialPartition/SPSystem.h"

//Include Scripts
#include "RotateScript.h"
#include "ScaleScript.h"
#include "PlayerCameraScript.h"
#include "PlayerMovementScript.h"
#include "TranslateScript.h"
#include "PivotScript.h"
#include "DebugControlsScript.h"

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
	GameObject* ground = &GameObjectFactory::CreatePlane(name, "Ground");
	ground->GetComponent<Transform>().SetLocalPosition(0, 0, 0);
	ground->GetComponent<Transform>().SetLocalScale(100, 100 ,100);
	ground->GetComponent<Transform>().IgnoreSpatialPartition(true);
	ground->GetComponent<MeshHolder>().textureList.textureArray[0] = TextureManager::GetInstance().AddTexture("Test Texture", "Image//Default//Test_Texture.tga");

	//LOD Sphere
	Mesh* meshLowLOD = MeshBuilder::GetInstance().GenerateSphere("Mesh Low LOD", Color(0, 1, 1), 6, 6, 0.5f);
	Mesh* meshMidLOD = MeshBuilder::GetInstance().GenerateSphere("Mesh Mid LOD", Color(0, 1, 0), 8, 8, 0.5f);
	Mesh* meshHighLOD = MeshBuilder::GetInstance().GenerateSphere("Mesh High LOD", Color(1, 0, 0), 64, 64, 0.5f);
	GameObject* sphereLOD = &GameObjectFactory::CreateEmpty(name, "Sphere LOD");
	sphereLOD->AddComponent<LODMeshHolder>().SetLODMesh(meshLowLOD, meshMidLOD, meshHighLOD);
	sphereLOD->GetComponent<MeshRenderer>().lightEnabled = true;
	sphereLOD->GetComponent<Transform>().SetLocalScale(5, 5, 5);
	sphereLOD->GetComponent<Transform>().SetLocalPosition(20, 5, 20);
	
	//SpatialPartition
	int xGridSize = 5; int yGridSize = 5; int zGridSize = 5;
	int xNumGrid = 13; int yNumGrid = 7; int zNumGrid = 13;
	SpatialPartitionSystem::GetInstance().CreateSpatialPartition(name)->Set(xGridSize, yGridSize, zGridSize, xNumGrid, yNumGrid, zNumGrid, 0, ((yNumGrid >> 1) - 1) * yGridSize, 0);
	//SpatialPartitionSystem::GetInstance().CreateSpatialPartition(name)->Set(xGridSize, yGridSize, zGridSize, xNumGrid, yNumGrid, zNumGrid);

}

void SceneTest::Update(double deltaTime) {	
	GameObjectManager::GetInstance().UpdateScripts(name, deltaTime);
	SpatialPartitionSystem::GetInstance().Update(name);
	//Close da app
	if (InputManager::GetInstance().GetInputInfo().keyDown[INPUT_QUIT]) {
		Application::GetInstance().Quit();
	}
	RenderSystem::GetInstance().Update(name, deltaTime);

	debugCountdown -= deltaTime;
	
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
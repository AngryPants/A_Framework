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
	int xGridSize = 20; int yGridSize = 20; int zGridSize = 20;
	int xNumGrid = 16; int yNumGrid = 2; int zNumGrid = 16;
	SpatialPartitionSystem::GetInstance().CreateSpatialPartition(name)->Set(xGridSize, yGridSize, zGridSize, xNumGrid, yNumGrid, zNumGrid, 0, ((yNumGrid >> 1) - 1) * yGridSize, 0);

	//Player
	player = &GameObjectFactory::CreateEmpty(name, "Player");
	player->CreateScript<PlayerMovementScript>();
	player->GetComponent<Transform>().SetLocalPosition(0, 0, -5);

	//Camera
	GameObject* camera = &GameObjectFactory::CreateCamera(name);
	camera->SetParent(*player);
	camera->GetComponent<Transform>().SetLocalPosition(0, 1, 0);
	camera->CreateScript<PlayerCameraScript>();
	
	//Lights
	GameObject* light = &GameObjectFactory::CreateLight(name);
	light->GetComponent<Light>().type = Light::LIGHT_TYPE::LIGHT_DIRECTIONAL;
	light->GetComponent<Light>().power = 5.0f;
	light->GetComponent<Transform>().SetLocalPosition(0, 0, 5);
	light->GetComponent<Transform>().SetLocalRotation(45, 0, 0);

	//Cube
	GameObject* cube = &GameObjectFactory::CreateCube(name, "Cube 1");
	cube->GetComponent<Transform>().SetLocalPosition(5, 15, 0);
	cube->GetComponent<MeshHolder>().textureList.textureArray[0] = TextureManager::GetInstance().AddTexture("Test Cube", "Image//Default//Test_Cube.tga");
	cube->CreateScript<RotateScript>();
	cube->CreateScript<TranslateScript>();

	//Camera Pivot
	GameObject* camPivot = &GameObjectFactory::CreateEmpty(name, "Camera Pivot");
	camPivot->SetParent(*cube);
	camPivot->GetComponent<Transform>().SetLocalPosition(0, 1, 0);
	camPivot->CreateScript<PivotScript>();

	//Camera2
	GameObject* camera2 = &GameObjectFactory::CreateCamera(name, "Camera 2");
	camera2->SetParent(*camPivot);
	camera2->GetComponent<Transform>().SetLocalPosition(0, 0, -2);
	camera2->GetComponent<Transform>().SetLocalRotation(30, 0, -2);
	camera2->GetComponent<Camera>().depth = -1.0f;

	//Cube 2
	GameObject* cube2 = &GameObjectFactory::CreateCube(name, "Cube 2");
	cube2->SetParent(*camPivot);
	cube2->GetComponent<Transform>().SetLocalPosition(0, 0, -2);
	cube2->CreateScript<ScaleScript>();
	cube2->CreateScript<RotateScript>();

	//Cube 3
	GameObject* cylinder = &GameObjectFactory::CreateCylinder(name, "Cylinder");
	cylinder->SetParent(*cube2);
	cylinder->GetComponent<Transform>().SetLocalPosition(2, -2, 4);
	cylinder->CreateScript<TranslateScript>();
	cylinder->CreateScript<RotateScript>();

	//Plane
	GameObject* plane = &GameObjectFactory::CreatePlane(name, "Plane");
	plane->GetComponent<Transform>().SetLocalScale(100, 100 ,100);
	plane->GetComponent<MeshHolder>().textureList.textureArray[0] = TextureManager::GetInstance().AddTexture("Test Texture", "Image//Default//Test_Texture.tga");


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
		SpatialPartitionSystem::GetInstance().GetSpatialPartition(name)->PrintSelf();
		debugCountdown = 2.f;
	}
}

void SceneTest::Exit() {	
	RenderHelper::GetInstance().DeleteShader(SHADERS::PHONG);
	GameObjectManager::GetInstance().Clear(name);
}
#include "SceneSplash.h"
#include "../Graphics/GraphicsManager.h"
#include "../Graphics/RenderHelper.h"
#include "../Application/Application.h"
#include "../Input/InputManager.h"
#include "../Mesh/MeshBuilder.h"
#include "../Texture/TextureManager.h"
#include "../Texture/TextureList.h"
#include "../Systems/Rendering//RenderSystem.h"
#include "../Systems/SpatialPartition/SPSystem.h"
#include "../Scene/SceneManager.h"
#include "../Systems/Physics/PhysicsSystem.h"

//Constructor(s) & Destructor
SceneSplash::SceneSplash(const string& _name) : Scene(_name) {
}

SceneSplash::~SceneSplash() {
}

void SceneSplash::Init() {
	//Initialise some GraphicsManager stuff.
	GraphicsManager::GetInstance().Enable<GraphicsManager::MODE::BLENDING>();
	//GraphicsManager::GetInstance().Enable<GraphicsManager::MODE::CULLING>();
	GraphicsManager::GetInstance().Enable<GraphicsManager::MODE::DEPTH_TEST>();
	GraphicsManager::GetInstance().SetBackgroundColor(0.0f, 0.0f, 0.0f, 1.0f);

	//Initialise the Shader.
	RenderHelper::GetInstance().LoadShader(SHADERS::PHONG);
	RenderHelper::GetInstance().EnableLight(true);
	RenderHelper::GetInstance().SetNumLights(8);
	RenderHelper::GetInstance().EnableFog(false);
	RenderHelper::GetInstance().SetAlphaDiscardValue(0.1f);

	//SpatialPartition 
	int xGridSize = 5; int yGridSize = 5; int zGridSize = 5;
	int xNumGrid = 5; int yNumGrid = 5; int zNumGrid = 5;
	//int xNumGrid = 0; int yNumGrid = 0; int zNumGrid = 0;
	SpatialPartitionSystem::GetInstance().CreateSpatialPartition(name)->Set(xGridSize, yGridSize, zGridSize, xNumGrid, yNumGrid, zNumGrid, 0, ((yNumGrid >> 1) - 1) * yGridSize, 0);

	//Lights
	GameObject* light = &GameObjectFactory::CreateLight(name);
	light->GetComponent<Light>().type = Light::LIGHT_TYPE::LIGHT_POINT;
	light->GetComponent<Light>().power = 1.0f;
	light->GetComponent<Transform>().SetLocalPosition(0, 0, 0);
	light->GetComponent<Transform>().SetLocalRotation(0, 180, 0);

	//Logo
	GameObject* logo = &GameObjectFactory::CreateQuad(name, "Logo");
	logo->GetComponent<MeshHolder>().textureList.textureArray[0] = TextureManager::GetInstance().AddTexture("Logo", "Image//Others//Logo.tga");

	//Text
	/*GameObject* textObject = &GameObjectFactory::CreateText(name, "Text Object");
	textObject->GetComponent<TextRenderer>().isUI = false;
	textObject->GetComponent<TextRenderer>().text = "Text Object";	
	textObject->GetComponent<TextRenderer>().scale.Set(0.05f, 0.1f, 0.1f);
	textObject->GetComponent<TextRenderer>().mesh = MeshBuilder::GetInstance().GenerateText("Text", 16, 16);
	textObject->GetComponent<TextRenderer>().textureList.textureArray[0] = TextureManager::GetInstance().AddTexture("Text", "Image//Fonts//Consolas.tga");
	textObject->CreateScript<RotateScript>(0);*/

	//Camera
	GameObject* camera = &GameObjectFactory::CreateCamera(name,"Camera");
	camera->GetComponent<Transform>().SetLocalPosition(0, 0, 1);
	camera->GetComponent<Transform>().SetLocalRotation(0, 180, 0);

	splashTimer = 1.0f;
}

void SceneSplash::Update(double _deltaTime) {
	SpatialPartitionSystem::GetInstance().Update(name);

	PhysicsSystem::GetInstance().UpdateDeltaTime(name, _deltaTime);
	GameObjectManager::GetInstance().UpdateScripts(name, _deltaTime);
	PhysicsSystem::GetInstance().Update(name);

	RenderSystem::GetInstance().Update(name, _deltaTime);

	//Close da app
	if (InputManager::GetInstance().GetInputInfo().keyDown[INPUT_QUIT]) {
		Application::GetInstance().Quit();
	}

	splashTimer -= _deltaTime;
	if (splashTimer < 0.0f) {
		//SceneManager::GetInstance().SetActiveScene("Assignment 1 Scene");
		//SceneManager::GetInstance().RemoveScene(name);
		cout << "Switching Scenes" << endl;
	}
}

void SceneSplash::Render() {
	SpatialPartitionSystem::GetInstance().Update(name);
	GraphicsManager::GetInstance().Enable<GraphicsManager::MODE::DEPTH_TEST>();	
	RenderSystem::GetInstance().Render(name, nullptr);
}

void SceneSplash::Exit() {
	RenderHelper::GetInstance().DeleteShader(SHADERS::PHONG);
	GameObjectManager::GetInstance().Clear(name);
}
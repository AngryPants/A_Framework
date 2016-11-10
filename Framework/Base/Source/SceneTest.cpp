#include "SceneTest.h"
#include "GraphicsManager.h"
#include "RenderHelper.h"
#include "Application.h"
#include "InputManager.h"
#include "MeshBuilder.h"
#include "TextureManager.h"
#include "RenderSystem.h"
#include "CubeScript.h"

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

	camera = &GameObjectFactory::CreateCamera(name);
	camera->GetComponent<Transform>().SetPosition(0, 3, 0);
	camera->GetComponent<Transform>().SetRotation(45, 0, 0);
	light = &GameObjectFactory::CreateLight(name);
	light->GetComponent<Light>().type = Light::LIGHT_TYPE::LIGHT_DIRECTIONAL;
	light->GetComponent<Light>().power = 5.0f;
	light->GetComponent<Transform>().SetPosition(0, 0, 5);
	light->GetComponent<Transform>().SetRotation(45, 0, 0);

	cube = &GameObjectFactory::CreateEmpty(name);
	cube->AddComponent<Transform>().SetPosition(0, 0, 5);
	cube->AddComponent<MeshRenderer>().mesh = MeshBuilder::GetInstance().GenerateOBJ("Cube", "OBJ//Default//Cube.obj");
	cube->GetComponent<MeshRenderer>().textureList.textureArray[0] = TextureManager::GetInstance().AddTexture("Test Cube", "Image//Default//Test_Cube.tga");
	CubeScript* cubeScript = new CubeScript();
	cubeScript->go = cube;
	cube->scripts[0] = cubeScript;
	
}

void SceneTest::Update(double deltaTime) {
	
	camera->GetComponent<Camera>().aspectRatio.Set(Application::GetInstance().GetWindowWidth(), Application::GetInstance().GetWindowHeight());
	//camera->GetComponent<Transform>().LookAt(light->GetComponent<Transform>().GetPosition(), Vector3(0, 1, 0));
	
	GameObjectManager::GetInstance().UpdateScripts(name, deltaTime);

	//Close da app
	if (InputManager::GetInstance().GetInputInfo().keyDown[INPUT_QUIT]) {
		Application::GetInstance().Quit();
	}

}

void SceneTest::Render() {	
	GraphicsManager::GetInstance().ClearColor();
	GraphicsManager::GetInstance().Enable<GraphicsManager::MODE::DEPTH_TEST>();

	RenderSystem::GetInstance().Render(name);

	//Render Entities UI
	GraphicsManager::GetInstance().Disable<GraphicsManager::MODE::DEPTH_TEST>();
	GraphicsManager::GetInstance().SetToUI();

	RenderSystem::GetInstance().RenderUI(name);
}

void SceneTest::Exit() {
	
	RenderHelper::GetInstance().DeleteShader(SHADERS::PHONG);
	GameObjectManager::GetInstance().Clear(name);

}
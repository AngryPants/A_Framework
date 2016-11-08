#include "SceneTest.h"
#include "GraphicsManager.h"
#include "RenderHelper.h"
#include "Application.h"
#include "InputManager.h"
#include "MeshBuilder.h"
#include "TextureManager.h"
#include "RenderSystem.h"

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
	GraphicsManager::GetInstance().Enable<GraphicsManager::MODE::DEPTH_TEST>();

	//Initialise the Shader.
	RenderHelper::GetInstance().LoadShader(SHADERS::PHONG);
	RenderHelper::GetInstance().EnableLight(true);
	RenderHelper::GetInstance().SetNumLights(1);
	RenderHelper::GetInstance().EnableFog(false);
	RenderHelper::GetInstance().SetAlphaDiscardValue(0.1f);

	camera = &GameObjectFactory::CreateCamera(name);
	light = &GameObjectFactory::CreateLight(name);
	light->GetComponent<Light>().type = Light::LIGHT_TYPE::LIGHT_DIRECTIONAL;
	light->GetComponent<Transform>().SetPosition(0, 0, 5);

	light->AddComponent<MeshRenderer>().mesh = MeshBuilder::GetInstance().GenerateOBJ("Cube", "OBJ//Default//Cube.obj");
	light->GetComponent<MeshRenderer>().textureList.textureArray[0] = TextureManager::GetInstance().AddTexture("Test Cube", "Image//Default//Test_Cube.tga");

}

void SceneTest::Update(double deltaTime) {
	
	camera->GetComponent<Camera>().aspectRatio.Set(Application::GetInstance().GetWindowWidth(), Application::GetInstance().GetWindowHeight());
	camera->GetComponent<Transform>().LookAt(light->GetComponent<Transform>().GetPosition(), Vector3(0, 1, 0));

	float speed = 3.0f;
	if (InputManager::GetInstance().GetInputInfo().keyDown[INPUT_BACKWARD]) {
		light->GetComponent<Transform>().Translate(0, 0, -speed * deltaTime);
	}
	if (InputManager::GetInstance().GetInputInfo().keyDown[INPUT_FORWARD]) {
		light->GetComponent<Transform>().Translate(0, 0, speed * deltaTime);
	}
	if (InputManager::GetInstance().GetInputInfo().keyDown[INPUT_LEFT]) {
		light->GetComponent<Transform>().Translate(speed * deltaTime, 0, 0);
	}
	if (InputManager::GetInstance().GetInputInfo().keyDown[INPUT_RIGHT]) {
		light->GetComponent<Transform>().Translate(-speed * deltaTime, 0, 0);
	}
	if (InputManager::GetInstance().GetInputInfo().keyDown[INPUT_UP]) {
		light->GetComponent<Transform>().Translate(0, speed * deltaTime, 0);
	}
	if (InputManager::GetInstance().GetInputInfo().keyDown[INPUT_DOWN]) {
		light->GetComponent<Transform>().Translate(0, -speed * deltaTime, 0);
	}
	if (InputManager::GetInstance().GetInputInfo().keyDown[INPUT_RESET]) {
		light->GetComponent<Transform>().SetPosition(0, 0, 0);
	}

	//Close da app
	if (InputManager::GetInstance().GetInputInfo().keyDown[INPUT_QUIT]) {
		Application::GetInstance().Quit();
	}

}

void SceneTest::Render() {

	GraphicsManager::GetInstance().ClearColor();
	GraphicsManager::GetInstance().SetToCameraView(camera->GetComponent<Camera>(), camera->GetComponent<Transform>());
	GraphicsManager::GetInstance().Enable<GraphicsManager::MODE::DEPTH_TEST>();
	RenderHelper::GetInstance().UpdateLight(light->GetComponent<Light>(), light->GetComponent<Transform>(), 0);
	
	RenderSystem::GetInstance().Render(name);
	
	//Render Entities UI
	GraphicsManager::GetInstance().Disable<GraphicsManager::MODE::DEPTH_TEST>();
	GraphicsManager::GetInstance().SetToUI();

}

void SceneTest::Exit() {
	
	RenderHelper::GetInstance().DeleteShader(SHADERS::PHONG);
	GameObjectManager::GetInstance().Clear(name);

}
#include "SceneTest.h"
#include "GraphicsManager.h"
#include "RenderHelper.h"
#include "Application.h"
#include "InputManager.h"
#include "MeshBuilder.h"
#include "TextureManager.h"

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
	
	mesh = MeshBuilder::GetInstance().GenerateOBJ("Cube", "OBJ//Default//Cube.obj");
	textureList.textureArray[0] = TextureManager::GetInstance().AddTexture("Test Cube", "Image//Default//Test_Cube.tga");
	
	camera = &GameObjectFactory::CreateCamera(name);
	light = &GameObjectFactory::CreateLight(name);
	light->GetComponent<Light>().type = Light::LIGHT_TYPE::LIGHT_POINT;

}

void SceneTest::Update(double deltaTime) {
	
	GameObjectManager::GetInstance().Update();

	camera->GetComponent<Camera>().aspectRatio.Set(Application::GetInstance().GetWindowWidth(), Application::GetInstance().GetWindowHeight());
	
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
	
	MS& modelStack = GraphicsManager::GetInstance().modelStack;
	modelStack.PushMatrix();
		modelStack.Translate(0, 0, 3);
		modelStack.Rotate(45, 0, 1, 0);
		modelStack.Rotate(45, 1, 0, 0);
		RenderHelper::GetInstance().RenderMesh(*mesh, textureList, true);
	modelStack.PopMatrix();
	
	//Render Entities UI
	GraphicsManager::GetInstance().Disable<GraphicsManager::MODE::DEPTH_TEST>();
	GraphicsManager::GetInstance().SetToUI();

}

void SceneTest::Exit() {
	
	RenderHelper::GetInstance().DeleteShader(SHADERS::PHONG);
	GameObjectManager::GetInstance().Clear(name);

}
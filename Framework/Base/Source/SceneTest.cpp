#include "SceneTest.h"
#include "GraphicsManager.h"
#include "RenderHelper.h"
#include "Application.h"

//Constructor(s) & Destructor
SceneTest::SceneTest(const string& name) : Scene(name) {
}

SceneTest::~SceneTest() {
}

//Interface Function(s)
void SceneTest::Init() {

	//Initialise some GraphicsManager stuff.
	GraphicsManager::GetInstance().Enable<GraphicsManager::MODE::BLENDING>();
	//GraphicsManager::GetInstance().Enable<GraphicsManager::MODE::CULLING>();
	GraphicsManager::GetInstance().Disable<GraphicsManager::MODE::CULLING>();
	GraphicsManager::GetInstance().Enable<GraphicsManager::MODE::DEPTH_TEST>();

	//Initialise the Shader.
	RenderHelper::GetInstance().LoadShader<SHADERS::SHADOW>();
	RenderHelper::GetInstance().EnableLight(false);
	RenderHelper::GetInstance().SetNumLights(0);
	RenderHelper::GetInstance().EnableFog(false);
	RenderHelper::GetInstance().SetAlphaDiscardValue(0.1f);

	entityManager = new EntityManager();
	//entityTest = new EntityTest("Entity A");
	EntityTest* entity = new EntityTest("Entity A");
	entityManager->AddEntity(*entity);
	camera = new Camera("Coolest camera in da world.");

	camera->SetPosition(Vector3(0, 0, 1));
	camera->SetTarget(Vector3(0, 0, 0));
	camera->SetUp(Vector3(0, 1, 0));

}

void SceneTest::Update(double deltaTime) {

	camera->aspectRatio.Set(Application::GetInstance().GetWindowWidth(), Application::GetInstance().GetWindowHeight());

}

void SceneTest::Render() {

	GraphicsManager::GetInstance().ClearColor();
	GraphicsManager::GetInstance().SetToCameraView(*camera);
	GraphicsManager::GetInstance().Enable<GraphicsManager::MODE::DEPTH_TEST>();
	
	//Render Entities
	entityManager->Render();

	//Render Entities UI
	entityManager->RenderUI();
}

void SceneTest::Exit() {

	delete camera;
}
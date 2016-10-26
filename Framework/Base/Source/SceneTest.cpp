#include "SceneTest.h"
#include "GraphicsManager.h"
#include "RenderHelper.h"

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

	entityTest = new EntityTest("Entity A");
	camera = new Camera("Coolest camera in da world.");

	camera->SetPosition(Vector3(0, 0, 1));
	camera->SetTarget(Vector3(0, 0, 0));
	camera->SetUp(Vector3(0, 1, 0));

}

void SceneTest::Update(double deltaTime) {
}

void SceneTest::Render() {

	GraphicsManager::GetInstance().ClearColor();
	GraphicsManager::GetInstance().SetToCameraView(*camera);
	GraphicsManager::GetInstance().Enable<GraphicsManager::MODE::DEPTH_TEST>();
	
	//Render Entities
	entityTest->Render();

}

void SceneTest::Exit() {

	delete entityTest;
	delete camera;

}
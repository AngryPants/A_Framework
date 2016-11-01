#include "SceneTest.h"
#include "GraphicsManager.h"
#include "RenderHelper.h"
#include "Application.h"
#include "InputManager.h"
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
	RenderHelper::GetInstance().LoadShader<SHADERS::SHADOW>();
	RenderHelper::GetInstance().EnableLight(true);
	RenderHelper::GetInstance().SetNumLights(1);
	RenderHelper::GetInstance().EnableFog(false);
	RenderHelper::GetInstance().SetAlphaDiscardValue(0.1f);

	entity = new EntityTest("Entity A");
	entity->position.Set(0, 0, 5);
	entity->rotation.Set(0, 180, 0);
	EntityManager::GetInstance().AddEntity(*entity);
	camera = new Camera("Camera");
	EntityManager::GetInstance().AddEntity(*camera); 

	camera->SetPosition(0, 0, 0);
	camera->SetRotation(0, 0, 0);
	camera->SetPerspective();
	
	backgroundMesh = MeshBuilder::GetInstance().GenerateQuad("Background");
	backgroundTexture.textureArray[0] = TextureManager::GetInstance().AddTexture("Background", "Image//Default//Texture_Test.tga");

	//lights[0].type = Light::LIGHT_TYPE::LIGHT_DIRECTIONAL;
	//lights[0].type = Light::LIGHT_TYPE::LIGHT_SPOT;
	lights[0].type = Light::LIGHT_TYPE::LIGHT_POINT;
	lights[0].transform.SetPosition(0, 0, 0);
	lights[0].transform.SetRotation(0, 0, 0);
	lights[0].power = 1.0f;
	//lights[0].color.Set(1, 0, 0);
	lightBall = MeshBuilder::GetInstance().GenerateOBJ("Light Ball", "OBJ\\Default\\Sphere.obj");

}

void SceneTest::Update(double deltaTime) {

	camera->aspectRatio.Set(Application::GetInstance().GetWindowWidth(), Application::GetInstance().GetWindowHeight());
	camera->LookAt(entity->position, Vector3(0, 1, 0));
	
	//Update Entities
	EntityManager::GetInstance().Update(deltaTime);

	float translationSpeed = 5.0f;
	if (InputManager::GetInstance().GetInputInfo().keyDown[INPUT_LEFT]) {
		//entity->position.x += translationSpeed * deltaTime;
		lights[0].transform.Translate(translationSpeed * deltaTime);
	}
	if (InputManager::GetInstance().GetInputInfo().keyDown[INPUT_RIGHT]) {
		//entity->position.x -= translationSpeed * deltaTime;
		lights[0].transform.Translate(-translationSpeed * deltaTime);
	}
	if (InputManager::GetInstance().GetInputInfo().keyDown[INPUT_UP]) {
		//entity->position.y += translationSpeed * deltaTime;
		lights[0].transform.Translate(0, translationSpeed * deltaTime, 0);
	}
	if (InputManager::GetInstance().GetInputInfo().keyDown[INPUT_DOWN]) {
		//entity->position.y -= translationSpeed * deltaTime;
		lights[0].transform.Translate(0, -translationSpeed * deltaTime, 0);
	}
	if (InputManager::GetInstance().GetInputInfo().keyDown[INPUT_ROLL_LEFT]) {
		//entity->position.z += translationSpeed * deltaTime;
		lights[0].transform.Translate(0, 0, translationSpeed * deltaTime);
	}
	if (InputManager::GetInstance().GetInputInfo().keyDown[INPUT_ROLL_RIGHT]) {
		//entity->position.z -= translationSpeed * deltaTime;
		lights[0].transform.Translate(0, 0, -translationSpeed * deltaTime);
	}
	if (InputManager::GetInstance().GetInputInfo().keyDown[INPUT_RESET]) {
		entity->position.Set(0, 0, 5);
		lights[0].transform.SetPosition(0, 0, 0);
		lights[0].transform.SetRotation(0, 0, 0);
	}
	//cout << "Light Forward: " << lights[0].transform.GetForward() << endl;
	//cout << "Light Position: " << lights[0].transform.GetPosition() << endl;

	//Close da app
	if (InputManager::GetInstance().GetInputInfo().keyDown[INPUT_QUIT]) {
		Application::GetInstance().Quit();
	}

}

void SceneTest::Render() {

	GraphicsManager::GetInstance().ClearColor();
	GraphicsManager::GetInstance().SetToCameraView(*camera);
	GraphicsManager::GetInstance().Enable<GraphicsManager::MODE::DEPTH_TEST>();
	/*for (unsigned int i = 0; i < RenderHelper::ShadowUniforms::MAX_LIGHTS; ++i) {
		RenderHelper::GetInstance().UpdateLight(lights[i], i);
	}*/
	RenderHelper::GetInstance().UpdateLight(lights[0], 0);

	//Render Entities
	MS& modelStack = GraphicsManager::GetInstance().modelStack;
	modelStack.PushMatrix();
		modelStack.Translate(0, 0, 10);
		modelStack.Rotate(180, 0, 1, 0);
		modelStack.Scale(100, 100, 1);
		RenderHelper::GetInstance().RenderMesh(*backgroundMesh, backgroundTexture, true);
	modelStack.PopMatrix();
	modelStack.PushMatrix();
		modelStack.Translate(lights[0].transform.GetPosition().x, lights[0].transform.GetPosition().y, lights[0].transform.GetPosition().z);
		modelStack.Scale(0.1f, 0.1f, 0.1f);
		RenderHelper::GetInstance().RenderMesh(*lightBall, false);
	modelStack.PopMatrix();
	EntityManager::GetInstance().Render();
	
	//Render Entities UI
	GraphicsManager::GetInstance().Disable<GraphicsManager::MODE::DEPTH_TEST>();
	GraphicsManager::GetInstance().SetToHUD(-10, 10, -10, 10, -10, 10);
	EntityManager::GetInstance().RenderUI();
}

void SceneTest::Exit() {
	EntityManager::GetInstance().Clear();
}
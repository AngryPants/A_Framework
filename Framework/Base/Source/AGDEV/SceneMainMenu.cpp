#include "SceneMainMenu.h"
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

//Include Scripts
#include "MainMenuScript.h"
#include "AIChaseScript.h"

SceneMainMenu::SceneMainMenu(const string& _name) : Scene(_name) {	
}

SceneMainMenu::~SceneMainMenu() {
}

//Virtual Function(s)
void SceneMainMenu::Init() {
	//Initialise some GraphicsManager stuff.
	GraphicsManager::GetInstance().Enable<GraphicsManager::MODE::BLENDING>();
	GraphicsManager::GetInstance().Enable<GraphicsManager::MODE::CULLING>();
	GraphicsManager::GetInstance().Enable<GraphicsManager::MODE::DEPTH_TEST>();
	GraphicsManager::GetInstance().SetBackgroundColor(0.0f, 0.0f, 0.4f, 1.0f);

	//Initialise the Shader.
	RenderHelper::GetInstance().LoadShader(SHADERS::PHONG);
	RenderHelper::GetInstance().EnableLight(true);
	RenderHelper::GetInstance().SetNumLights(8);
	RenderHelper::GetInstance().EnableFog(false);
	RenderHelper::GetInstance().SetAlphaDiscardValue(0.1f);

	//Set Skybox Textures
	skybox.textureLists[Skybox::TEXTURES::SKYBOX_TOP].textureArray[0] = TextureManager::GetInstance().AddTexture("Skybox Top", "Image//Skybox//Mountains//Top.tga");
	skybox.textureLists[Skybox::TEXTURES::SKYBOX_BOTTOM].textureArray[0] = TextureManager::GetInstance().AddTexture("Skybox Bottom", "Image//Skybox//Mountains//Bottom.tga");
	skybox.textureLists[Skybox::TEXTURES::SKYBOX_BACK].textureArray[0] = TextureManager::GetInstance().AddTexture("Skybox Back", "Image//Skybox//Mountains//Back.tga");
	skybox.textureLists[Skybox::TEXTURES::SKYBOX_LEFT].textureArray[0] = TextureManager::GetInstance().AddTexture("Skybox Left", "Image//Skybox//Mountains//Left.tga");
	skybox.textureLists[Skybox::TEXTURES::SKYBOX_FRONT].textureArray[0] = TextureManager::GetInstance().AddTexture("Skybox Front", "Image//Skybox//Mountains//Front.tga");
	skybox.textureLists[Skybox::TEXTURES::SKYBOX_RIGHT].textureArray[0] = TextureManager::GetInstance().AddTexture("Skybox Right", "Image//Skybox//Mountains//Right.tga");

	//SpatialPartition 
	int xGridSize = 1; int yGridSize = 1; int zGridSize = 1;
	int xNumGrid = 1; int yNumGrid = 1; int zNumGrid = 1;
	//int xNumGrid = 0; int yNumGrid = 0; int zNumGrid = 0;
	SpatialPartitionSystem::GetInstance().CreateSpatialPartition(name)->Set(xGridSize, yGridSize, zGridSize, xNumGrid, yNumGrid, zNumGrid, 0, ((yNumGrid >> 1) - 1) * yGridSize, 0);

	//Lights
	GameObject* light = &GameObjectFactory::CreateLight(name);
	light->GetComponent<Light>().type = Light::LIGHT_TYPE::LIGHT_POINT;
	light->GetComponent<Light>().power = 5.0f;
	light->GetComponent<Transform>().SetLocalPosition(0, 10, 0);

	{
		GameObject& startGame = GameObjectFactory::CreateCube(name);
		startGame.GetComponent<Transform>().SetLocalScale(1, 1, 1);
		startGame.GetComponent<Transform>().SetLocalPosition(-1, 0, 0);

		GameObject& startGameText = GameObjectFactory::CreateText(name);	
		startGameText.SetParent(startGame);
		startGameText.GetComponent<Transform>().Translate(0, 0, 0.6f);
		startGameText.GetComponent<Transform>().SetLocalScale(0.15f, 0.15f, 0.15f);
		startGameText.GetComponent<TextRenderer>().text = "Start";
		startGameText.GetComponent<TextRenderer>().isUI = false;
		startGameText.GetComponent<TextRenderer>().centralise = true;
		startGameText.GetComponent<TextRenderer>().lightEnabled = false;		
		startGameText.GetComponent<TextRenderer>().textColor.Set(0, 0, 0);	
		startGameText.GetComponent<TextRenderer>().mesh = MeshBuilder::GetInstance().GenerateText("Text", 16, 16);
		startGameText.GetComponent<TextRenderer>().textureList.textureArray[0] = TextureManager::GetInstance().AddTexture("Text", "Image//Fonts//Consolas.tga");

		GameObject& quitGame = GameObjectFactory::CreateCube(name);
		quitGame.GetComponent<Transform>().SetLocalScale(1, 1, 1);
		quitGame.GetComponent<Transform>().SetLocalPosition(1, 0, 0);

		GameObject& quitGameText = GameObjectFactory::CreateText(name);	
		quitGameText.SetParent(quitGame);
		quitGameText.GetComponent<Transform>().Translate(0, 0, 0.6f);
		quitGameText.GetComponent<Transform>().SetLocalScale(0.15f, 0.15f, 0.15f);
		quitGameText.GetComponent<TextRenderer>().text = "Quit";
		quitGameText.GetComponent<TextRenderer>().isUI = false;
		quitGameText.GetComponent<TextRenderer>().centralise = true;
		quitGameText.GetComponent<TextRenderer>().lightEnabled = false;	
		quitGameText.GetComponent<TextRenderer>().textColor.Set(0, 0, 0);	
		quitGameText.GetComponent<TextRenderer>().mesh = MeshBuilder::GetInstance().GenerateText("Text", 16, 16);
		quitGameText.GetComponent<TextRenderer>().textureList.textureArray[0] = TextureManager::GetInstance().AddTexture("Text", "Image//Fonts//Consolas.tga");

		GameObject& optionsHandler = GameObjectFactory::CreateEmpty(name);
		optionsHandler.CreateScript<MainMenuScript>();
		MainMenuScript* script = static_cast<MainMenuScript*>(optionsHandler.GetScript(0));
		script->optionObjects[MainMenuScript::M_OPTION_START] = &startGame;
		script->optionObjects[MainMenuScript::M_OPTION_QUIT] = &quitGame;
	}

	//Ground
	GameObject& ground = GameObjectFactory::CreatePlane(name);
	ground.GetComponent<Transform>().SetLocalPosition(0.0f, -2.0f, 0.0f);
	ground.GetComponent<Transform>().SetLocalScale(10000.0f, 1.0f, 10000.0f);

	//Camera
	GameObject& camera = GameObjectFactory::CreateCamera(name,"Camera");
	camera.GetComponent<Transform>().SetLocalPosition(0, 0, 4);
	camera.GetComponent<Transform>().SetLocalRotation(0, 180, 0);
}

void SceneMainMenu::Update(double _deltaTime) {
	SpatialPartitionSystem::GetInstance().Update(name);

	PhysicsSystem::GetInstance().UpdateDeltaTime(name, _deltaTime);
	GameObjectManager::GetInstance().UpdateScripts(name, _deltaTime);
	PhysicsSystem::GetInstance().Update(name);

	RenderSystem::GetInstance().Update(name, _deltaTime);

	//Close da app
	if (InputManager::GetInstance().GetInputInfo().keyDown[INPUT_QUIT]) {
		Application::GetInstance().Quit();
	}	
}

void SceneMainMenu::Render() {
	//SpatialPartitionSystem::GetInstance().Update(name);
	GraphicsManager::GetInstance().Enable<GraphicsManager::MODE::DEPTH_TEST>();	
	RenderSystem::GetInstance().Render(name, &skybox);
}

void SceneMainMenu::Exit() {
	RenderHelper::GetInstance().DeleteShader(SHADERS::PHONG);
	GameObjectManager::GetInstance().Clear(name);
}
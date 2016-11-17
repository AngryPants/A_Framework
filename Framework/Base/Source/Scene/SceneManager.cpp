#include "SceneManager.h"
#include <iostream>
#include <exception>

using std::cout;
using std::endl;
using std::exception;

//Constructor(s) & Destructor
SceneManager::SceneManager() {
	activeScene = nullptr;
	nextScene = nullptr;
}

SceneManager::~SceneManager() {
	RemoveAllScenes();
}

//Private Function(s)
Scene* SceneManager::GetScene(const string& sceneName) {
	map<string, Scene*>::iterator mapIter = sceneMap.find(sceneName);
	if (mapIter == sceneMap.end()) {
		cout << ("Unable to GetScene(" + sceneName + ") as no scene with that name exists.") << endl;;
		return nullptr;
	}
	return mapIter->second;
}

//Interface Function(s)
void SceneManager::Update(const double deltaTime) {
	//Check if we need to switch scenes.
	if (nextScene != nullptr) {
		if (activeScene != nullptr) {
			activeScene->Save();
			activeScene->Exit();			
		}
		activeScene = nextScene;
		nextScene = nullptr;
		activeScene->Init();
		activeScene->Load();
	}

	//Now let's remove all of the scenes we need to remove.
	for (set<string>::iterator setIter = removeQueue.begin(); setIter != removeQueue.end(); ++setIter) {
		//We need to make sure that the activeScene is not going to be removed as well.
		//If it is, then we'll set activeScene to nullptr.
		if (*setIter == activeScene->name) {
			activeScene->Save();
			activeScene->Exit();
			activeScene = nullptr;
		}
		delete sceneMap.find(*setIter)->second;
		sceneMap.erase(*setIter);
	}
	removeQueue.clear();

	//Update the activeScene if it isn't a nullptr.
	if (activeScene != nullptr) {
		activeScene->Update(deltaTime);
	}
	
}

void SceneManager::Render() {
	if (activeScene != nullptr) {
		activeScene->Render();
	}
}

bool SceneManager::CheckIfSceneExist(const string& sceneName) {	
	return sceneMap.count(sceneName) != 0;
}

void SceneManager::RemoveScene(const string& sceneName) {
	if (!CheckIfSceneExist(sceneName)) {
		string errorMessage = "Unable to RemoveScene(" + sceneName + ") as no scene with that name exists.";
		cout << errorMessage << endl;
		throw exception(errorMessage.c_str());
	}
	removeQueue.insert(sceneName);
}

void SceneManager::SetActiveScene(const string& sceneName) {
	if (activeScene != nullptr && sceneName == activeScene->name) {
		cout << "Unable to SetActive(" + sceneName + ") as the scene is already active." << endl;
		return;
	}
	nextScene = GetScene(sceneName);
}

Scene* SceneManager::GetActiveScene() {
	return this->activeScene;
}

void SceneManager::RemoveAllScenes() {
	removeQueue.clear();
	if (activeScene != nullptr) {
		activeScene->Save();
		activeScene->Exit();
		activeScene = nullptr;
	}
	nextScene = nullptr;

	for (map<string, Scene*>::iterator mapIter = sceneMap.begin(); mapIter != sceneMap.end(); ++mapIter) {
		delete mapIter->second;
	}
	sceneMap.clear();
}
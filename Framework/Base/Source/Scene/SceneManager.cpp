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
Scene* SceneManager::GetScene(const string& _sceneName) {
	map<string, Scene*>::iterator mapIter = sceneMap.find(_sceneName);
	if (mapIter == sceneMap.end()) {
		cout << ("Unable to GetScene(" + _sceneName + ") as no scene with that name exists.") << endl;;
		return nullptr;
	}
	return mapIter->second;
}

//Interface Function(s)
void SceneManager::Update(double _deltaTime) {
	//Check if we need to switch scenes.
	if (nextScene != nullptr) {
		activeScene = nextScene;
		nextScene = nullptr;
	}

	activeScene->Init({});

	//Now let's remove all of the scenes we need to remove.
	for (set<string>::iterator setIter = removeQueue.begin(); setIter != removeQueue.end(); ++setIter) {
		//We need to make sure that the activeScene is not going to be removed as well.
		//If it is, then we'll set activeScene to nullptr.
		if (*setIter == activeScene->name) {
			activeScene = nullptr;
		}
		sceneMap.find(*setIter)->second->Exit({});
		delete sceneMap.find(*setIter)->second;
		sceneMap.erase(*setIter);
	}
	removeQueue.clear();

	//Update the activeScene if it isn't a nullptr.
	if (activeScene != nullptr) {
		activeScene->Update(_deltaTime, {});
	}
	
}

void SceneManager::Render() {
	if (activeScene != nullptr) {
		activeScene->Render({});
	}
}

bool SceneManager::CheckIfSceneExist(const string& _sceneName) {	
	return sceneMap.count(_sceneName) != 0;
}

void SceneManager::RemoveScene(const string& _sceneName) {
	if (!CheckIfSceneExist(_sceneName)) {
		string errorMessage = "Unable to RemoveScene(" + _sceneName + ") as no scene with that name exists.";
		cout << errorMessage << endl;
		throw exception(errorMessage.c_str());
	}
	removeQueue.insert(_sceneName);
}

void SceneManager::SetActiveScene(const string& _sceneName) {
	if (activeScene != nullptr && _sceneName == activeScene->name) {
		cout << "Unable to SetActive(" + _sceneName + ") as the scene is already active." << endl;
		return;
	}
	nextScene = GetScene(_sceneName);
}

Scene* SceneManager::GetActiveScene() {
	return this->activeScene;
}

void SceneManager::RemoveAllScenes() {
	removeQueue.clear();
	activeScene = nullptr;
	nextScene = nullptr;

	for (map<string, Scene*>::iterator mapIter = sceneMap.begin(); mapIter != sceneMap.end(); ++mapIter) {
		mapIter->second->Exit({});
		delete mapIter->second;
	}
	sceneMap.clear();
}

//Exit a scene without removing it.
void SceneManager::ExitScene(const string& _sceneName) {
	map<string, Scene*>::iterator mapIter = sceneMap.find(_sceneName);
	if (mapIter != sceneMap.end()) {
		mapIter->second->Exit({});
	}
}
#include "GameObjectManager.h"
#include "GameObject.h"

GameObjectManager::GameObjectManager() {
}

GameObjectManager::~GameObjectManager() {
	ClearAll();
}

GameObject& GameObjectManager::CreateGameObject(const string& space, const string& name) {
	//Create the GameObject.
	GameObject* goPtr = new GameObject(space, name);
	//Add to the addQueue.
	addQueue.insert(goPtr);

	return *goPtr;
}

void GameObjectManager::RemoveGameObject(GameObject& gameObject) {
	removeQueue.insert(&gameObject);
}

void GameObjectManager::AddGameObjects() {
	for (set<GameObject*>::iterator setIter = addQueue.begin(); setIter != addQueue.end(); ++setIter) {
		GameObject* goPtr = *setIter;
		map<string, set<GameObject*> >::iterator mapIter = goMap.find(goPtr->GetSpace());
		if (mapIter != goMap.end()) {
			mapIter->second.insert(goPtr);
		} else {
			set<GameObject*> goSet;
			goSet.insert(goPtr);
			goMap.insert(pair<string, set<GameObject*> >(goPtr->GetSpace(), goSet));
		}
	}
	addQueue.clear();
}

void GameObjectManager::RemoveGameObjects() {
	for (set<GameObject*>::iterator setIter = addQueue.begin(); setIter != addQueue.end(); ++setIter) {
		GameObject* goPtr = *setIter;
		map<string, set<GameObject*> >::iterator mapIter = goMap.find(goPtr->GetSpace());
		mapIter->second.erase(goPtr);
		delete goPtr;
	}
	removeQueue.clear();
}

void GameObjectManager::Clear(const string& space) {
	set<GameObject*>::iterator setIter = removeQueue.begin();
	while (setIter != removeQueue.end()) {
		GameObject* goPtr = *setIter;
		if (goPtr->GetSpace() == space) {
			setIter = removeQueue.erase(setIter);
		} else {
			++setIter;
		}
	}

	setIter = addQueue.begin();
	while (setIter != addQueue.end()) {
		GameObject* goPtr = *setIter;
		if (goPtr->GetSpace() == space) {
			setIter = addQueue.erase(setIter);
			delete goPtr;			
		} else {
			++setIter;
		}
	}

	map<string, set<GameObject*> >::iterator mapIter = goMap.find(space);
	if (mapIter != goMap.end()) {
		setIter = mapIter->second.begin();
		while (setIter != mapIter->second.end()) {
			GameObject* goPtr = *setIter;
			delete goPtr;
			++setIter;
		}
		goMap.erase(mapIter);
	}
}

void GameObjectManager::ClearAll() {
	
	while (!goMap.empty()) {
		Clear(goMap.begin()->first);
	}

}

void GameObjectManager::Update() {
	AddGameObjects();
	RemoveGameObjects();
}

void GameObjectManager::UpdateScripts(const string& space, const double deltaTime) {

	map<string, set<GameObject*> >::iterator mapIter = goMap.find(space);
	if (mapIter == goMap.end()) {
		cout << "Unable to UpdateScripts() as space is not found." << endl;
		return;
	}

	for (set<GameObject*>::iterator setIter = mapIter->second.begin(); setIter != mapIter->second.end(); ++setIter) {
		GameObject* go = *setIter;
		for (unsigned int i = 0; i < sizeof(go->scripts)/sizeof(go->scripts[0]); ++i) {
			if (go->scripts[i] != nullptr) {
				go->scripts[i]->Update(deltaTime);
			}
		}
	}

}

set<GameObject*>& GameObjectManager::GetGameObjects(const string& space) {

	map<string, set<GameObject*> >::iterator mapIter = goMap.find(space);
	if (mapIter == goMap.end()) {
		throw exception("Cannot GetGameObjects() as space is not found.");
	}

	return mapIter->second;
}
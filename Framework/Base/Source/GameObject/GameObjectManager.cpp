#include "GameObjectManager.h"
#include "GameObject.h"
#include "../Systems/SpatialPartition/SPSystem.h"

//Constructor(s) & Destructor
GameObjectManager::GameObjectManager() {
}

GameObjectManager::~GameObjectManager() {
	ClearAll();
}

//Interface Function(s)
GameObject& GameObjectManager::CreateGameObject(const string& space, const string& name) {
	//Create the GameObject.
	GameObject* goPtr = new GameObject(space, name, {});
	//Add to the addQueue.
	addQueue.insert(goPtr);
	//Add to our goVector
	if (goPtr->GetID() >= goVector.size()) {
		goVector.resize(goPtr->GetID() + 1, nullptr);
	}
	goVector[goPtr->GetID()] = goPtr;

	//SpatialPartition
	SpatialPartitionSystem::GetInstance().CreateSpatialPartition(space)->Add(goPtr->GetID());

	return *goPtr;
}

void GameObjectManager::RemoveGameObject(GameObject& _gameObject, PassKey<GameObject> _key) {
	removeQueue.insert(&_gameObject);
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
	for (set<GameObject*>::iterator setIter = removeQueue.begin(); setIter != removeQueue.end(); ++setIter) {
		GameObject* goPtr = *setIter;
		map<string, set<GameObject*> >::iterator mapIter = goMap.find(goPtr->GetSpace());
		mapIter->second.erase(goPtr);
		goVector[goPtr->GetID()] = nullptr;
		SpatialPartitionSystem::GetInstance().GetSpatialPartition(goPtr->GetSpace())->Remove(goPtr->GetID());
		//delete goPtr;
		goPtr->Delete({});
	}
	removeQueue.clear();
}

GameObject* GameObjectManager::GetGameObjectByID(GameObjectID id) {
	if (id >= goVector.size()) {
		return nullptr;
	}
	return goVector[id];
}

void GameObjectManager::Clear(const string& space) {
	//Remove them from our remove queue, since we're gonna delete it in the main set anyways.
	set<GameObject*>::iterator setIter = removeQueue.begin();
	while (setIter != removeQueue.end()) {
		GameObject* goPtr = *setIter;
		if (goPtr->GetSpace() == space) {
			setIter = removeQueue.erase(setIter);
		} else {
			++setIter;
		}
	}

	//Delete those in our add queue.
	setIter = addQueue.begin();
	while (setIter != addQueue.end()) {
		GameObject* goPtr = *setIter;
		if (goPtr->GetSpace() == space) {
			setIter = addQueue.erase(setIter);
			goVector[goPtr->GetID()] = nullptr;
			//delete goPtr;
			goPtr->Delete({});
		} else {
			++setIter;
		}
	}

	//Delete the main set.
	map<string, set<GameObject*> >::iterator mapIter = goMap.find(space);
	if (mapIter != goMap.end()) {
		setIter = mapIter->second.begin();
		while (setIter != mapIter->second.end()) {
			GameObject* goPtr = *setIter;
			goVector[goPtr->GetID()] = nullptr;
			//delete goPtr;
			goPtr->Delete({});
			++setIter;
		}
		goMap.erase(mapIter);
	}
}

//Clear Everything.
void GameObjectManager::ClearAll() {	
	while (!goMap.empty()) {
		Clear(goMap.begin()->first);
	}
}

//Add and remove whatever we need.
void GameObjectManager::Update() {
	AddGameObjects();
	RemoveGameObjects();
	while (!goVector.empty() && goVector.back() == nullptr) {
		goVector.pop_back();
	}
}

void GameObjectManager::UpdateScripts(const string& space, const double deltaTime) {
	//Check that the space exist.
	map<string, set<GameObject*> >::iterator mapIter = goMap.find(space);
	if (mapIter == goMap.end()) {
		cout << "Unable to UpdateScripts() as space is not found." << endl;
		return;
	}

	//Update the scripts.
	for (set<GameObject*>::iterator setIter = mapIter->second.begin(); setIter != mapIter->second.end(); ++setIter) {
		GameObject* go = *setIter;
		go->UpdateScripts(deltaTime, {});
	}
}

set<GameObject*> GameObjectManager::GetGameObjects(const string& space) {
	map<string, set<GameObject*> >::iterator mapIter = goMap.find(space);
	if (mapIter == goMap.end()) {
		throw exception("Cannot GetGameObjects() as space is not found.");
	}

	return mapIter->second;
}

int GameObjectManager::GetNumGameObjects(const string& space) {
	map<string, set<GameObject*> >::iterator mapIter = goMap.find(space);
	if (mapIter == goMap.end()) {
		return 0;
	}
	return mapIter->second.size();
}
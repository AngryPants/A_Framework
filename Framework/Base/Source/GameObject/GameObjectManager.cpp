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

		//SpatialPartition
		SpatialPartitionSystem::GetInstance().CreateSpatialPartition(goPtr->GetSpace())->Add(goPtr->GetID());
	}
	addQueue.clear();
}

void GameObjectManager::RemoveGameObjects() {
	for (set<GameObject*>::iterator setIter = removeQueue.begin(); setIter != removeQueue.end(); ++setIter) {
		GameObject* goPtr = *setIter;
		SpatialPartitionSystem::GetInstance().GetSpatialPartition(goPtr->GetSpace())->Remove(goPtr->GetID());

		map<string, set<GameObject*> >::iterator mapIter = goMap.find(goPtr->GetSpace());		
		mapIter->second.erase(goPtr);
		goVector[goPtr->GetID()] = nullptr;
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
	for (set<GameObject*>::iterator setIter = addQueue.begin(); setIter != addQueue.end(); ) {
		GameObject* goPtr = *setIter;
		if (goPtr->GetSpace() != space) {
			++setIter;
			continue;
		}

		map<string, set<GameObject*> >::iterator mapIter = goMap.find(goPtr->GetSpace());
		if (mapIter != goMap.end()) {
			mapIter->second.insert(goPtr);
		} else {
			set<GameObject*> goSet;
			goSet.insert(goPtr);
			goMap.insert(pair<string, set<GameObject*> >(goPtr->GetSpace(), goSet));
		}
		SpatialPartitionSystem::GetInstance().CreateSpatialPartition(goPtr->GetSpace())->Add(goPtr->GetID());

		setIter = addQueue.erase(setIter);
	}

	//Destroy them all. (Not Delete) (This is a work around as a SceneNode called GameObject::Destroy() when deleted).
	//This makes the GameObject called RemoveGameobject(). This will cause the GameObject to add itself to the removeQueue,
	//and since it is already deleted, it will crash.
	map<string, set<GameObject*> >::iterator mapIter = goMap.find(space);
	if (mapIter != goMap.end()) {
		for (set<GameObject*>::iterator setIter = mapIter->second.begin(); setIter != mapIter->second.end(); ++setIter) {
			GameObject* goPtr = *setIter;
			goPtr->Destroy();
		}
	}

	for (set<GameObject*>::iterator setIter = removeQueue.begin(); setIter != removeQueue.end(); ) {
		GameObject* goPtr = *setIter;
		if (goPtr->GetSpace() != space) {
			++setIter;
			continue;
		}

		SpatialPartitionSystem::GetInstance().GetSpatialPartition(goPtr->GetSpace())->Remove(goPtr->GetID());
		map<string, set<GameObject*> >::iterator mapIter = goMap.find(goPtr->GetSpace());		
		mapIter->second.erase(goPtr);
		goVector[goPtr->GetID()] = nullptr;
		goPtr->Delete({});
		setIter = removeQueue.erase(setIter);
	}

	//Delete the main set.
	if (mapIter != goMap.end()) {
		set<GameObject*>::iterator setIter = mapIter->second.begin();
		while (setIter != mapIter->second.end()) {
			GameObject* goPtr = *setIter;
			SpatialPartitionSystem::GetInstance().GetSpatialPartition(goPtr->GetSpace())->Remove(goPtr->GetID());
			goVector[goPtr->GetID()] = nullptr;
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
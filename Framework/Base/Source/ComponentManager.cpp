#include "ComponentManager.h"
#include "GameObject.h"

#include "Camera.h"
#include "MeshRenderer.h"
#include "Transform.h"
#include "Light.h"

//Helper Functions. I can't put them in the header since they are templates, and including GameObject.h in header will
//cause an include loop of non-compiling death.
void InsertComponent(Component& component, ComponentMap& componentMap, ComponentTypeID id) {
	//Get the TypeID of the Component.
	//cout << "Inserting Component of Type: " << typeid(component).name() << " ID: " << to_string(id) << endl;
	//Check if the space exist.
	ComponentMap::iterator mapIter = componentMap.find(component.GetSpace());
	if (mapIter != componentMap.end()) {
		mapIter->second[id].insert(&component); //Insert the component pointer.
	} else {
		array<set<Component*>, MAX_COMPONENTS> componentArray;
		componentArray[id].insert(&component);
		componentMap.insert(std::pair<string, array<set<Component*>, MAX_COMPONENTS> >(component.GetSpace(), componentArray));
	}
}

void DeleteComponent(Component& component, ComponentMap& componentMap, ComponentTypeID id) {
	//Remove the component from the componentMap.
	string space = component.GetSpace();
	componentMap.find(space)->second[id].erase(&component);
	delete &component;
}

//Constructor(s) & Destructor
ComponentManager::ComponentManager() {
}

ComponentManager::~ComponentManager() {
	ClearAll();
}

//Private Function(s)
void ComponentManager::AddComponents() {
	for (unsigned int i = 0; i < MAX_COMPONENTS; ++i) {
		for (set<Component*>::iterator setIter = addQueue[i].begin(); setIter != addQueue[i].end(); ++setIter) {
			Component* componentPtr = *setIter;
			InsertComponent(*componentPtr, componentMap, i);
		}
		addQueue[i].clear();
	}	
}

void ComponentManager::RemoveComponents() {
	for (unsigned int i = 0; i < MAX_COMPONENTS; ++i) {
		for (set<Component*>::iterator setIter = removeQueue[i].begin(); setIter != removeQueue[i].end(); ++setIter) {
			Component* componentPtr = *setIter;
			DeleteComponent(*componentPtr, componentMap, i);
		}
		removeQueue[i].clear();
	}
}

//Interface Function(s)
void ComponentManager::Update() {
	AddComponents();
	RemoveComponents();
}

void ComponentManager::Clear(const string& space) {
	//Clear the removeQueue.
	for (unsigned int i = 0; i < MAX_COMPONENTS; ++i) {
		for (set<Component*>::iterator setIter = removeQueue[i].begin(); setIter != removeQueue[i].end();) {
			Component* componentPtr = *setIter;
			if (componentPtr->GetSpace() != space) {
				++setIter;
			} else {
				setIter = removeQueue[i].erase(setIter);
			}
		}
	}

	//Clear the addQueue.
	for (unsigned int i = 0; i < MAX_COMPONENTS; ++i) {
		for (set<Component*>::iterator setIter = addQueue[i].begin(); setIter != addQueue[i].end();) {
			Component* componentPtr = *setIter;
			if (componentPtr->GetSpace() != space) {
				++setIter;
			} else {
				setIter = addQueue[i].erase(setIter);
				delete componentPtr;
			}
		}	
	}

	//Clear the main ComponentMap.
	ComponentMap::iterator mapIter = componentMap.find(space);
	if (mapIter != componentMap.end()) {
		for (unsigned int i = 0; i < MAX_COMPONENTS; ++i) {
			for (set<Component*>::iterator setIter = mapIter->second[i].begin(); setIter != mapIter->second[i].end(); ++setIter) {
				delete *setIter;
			}
			mapIter->second[i].clear();
		}
	}
	componentMap.erase(mapIter);
}

void ComponentManager::ClearAll() {
	while (!componentMap.empty()) {
		Clear(componentMap.begin()->first);
	}
}
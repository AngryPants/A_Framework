#include "ComponentManager.h"
#include "GameObject.h"

//Helper Functions. I can't put them in the header since they are templates, and including GameObject.h in header will
//cause an include loop of non-compiling death.
template <class Type>
void InsertComponent(Type& component, ComponentMap& componentMap) {
	//Get the TypeID of the Component.
	ComponentTypeID id = GetComponentTypeID<Type>();
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

template <class Type>
void DeleteComponent(Type& component, ComponentMap& componentMap) {
	//Get the TypeID of the Component.
	ComponentTypeID id = GetComponentTypeID<Type>();
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
	for (set<Component*>::iterator setIter = addQueue.begin(); setIter != addQueue.end(); ++setIter) {
		Component* componentPtr = *setIter;
		InsertComponent(*componentPtr, componentMap);
	}
	addQueue.clear();
}

void ComponentManager::RemoveComponents() {
	for (set<Component*>::iterator setIter = removeQueue.begin(); setIter != removeQueue.end(); ++setIter) {
		Component* componentPtr = *setIter;
		DeleteComponent(*componentPtr, componentMap);
	}
	removeQueue.clear();
}

//Interface Function(s)
void ComponentManager::Update() {
	AddComponents();
	RemoveComponents();
}

void ComponentManager::Clear(const string& space) {
	//Clear the removeQueue.
	for (set<Component*>::iterator setIter = removeQueue.begin(); setIter != removeQueue.end();) {
		Component* componentPtr = *setIter;
		if (componentPtr->GetSpace() != space) {
			++setIter;
		} else {
			setIter = removeQueue.erase(setIter);
		}
	}
	
	//Clear the addQueue.
	for (set<Component*>::iterator setIter = addQueue.begin(); setIter != addQueue.end();) {
		Component* componentPtr = *setIter;
		if (componentPtr->GetSpace() != space) {
			++setIter;
		} else {
			setIter = addQueue.erase(setIter);
			delete componentPtr;
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
#ifndef COMPONENT_MANAGER_H
#define COMPONENT_MANAGER_H

#include "SingletonTemplate.h"
#include "ComponentTypeID.h"
#include <set>
#include <map>
#include <string>
#include <array>

//Temporary
#include "Transform.h"

using namespace std;

//Fuck me, so many templates. I hope this thing runs.

class GameObject;

typedef map<string, array<set<Component*>, MAX_COMPONENTS> > ComponentMap;

class ComponentManager : public Singleton<ComponentManager> {

	friend class Singleton<ComponentManager>;

private:
	//Variable(s)
	//Welp, this looks complicated. I'm using std::array as a work around.
	ComponentMap componentMap;
	set<Component*> addQueue;
	set<Component*> removeQueue;

	//Constructor(s) & Destructor
	ComponentManager();
	virtual ~ComponentManager();

	//Private Function(s)
	void AddComponents();
	void RemoveComponents();
	
public:	
	//Interface Function(s)
	void Update();
	void Clear(const string& space);
	void ClearAll();

	template <class Type>
	Type& CreateComponent(GameObject& gameObject) {
		//Create the component.
		Type* componentPtr = new Type(gameObject);
		addQueue.insert(componentPtr);
		return *componentPtr;
	}

	template <class Type>
	void RemoveComponent(Type& component) {
		if (std::is_base_of<Component, Type>::value == false) {
			throw exception("Cannot remove non-component using RemoveComponent().");
		}
		removeQueue.insert(&component);
	}

	template <class Type>
	set<Component*>& GetComponents(const string& space) {
		if (componentMap.count(space) == 0) {
			array<set<Component*>, MAX_COMPONENTS> componentArray;
			componentMap.insert(std::pair<string, array<set<Component*>, MAX_COMPONENTS> >(space, componentArray));			
		}
		return componentMap.find(space)->second[GetComponentTypeID<Type>()];
	}

	/*template <class Type>
	Type& CreateComponent(GameObject& gameObject) {
		//Create the component.
		Type* componentPtr = new Type(gameObject);
		//Get the TypeID of the Component.
		ComponentTypeID id = GetComponentTypeID<Type>();
		//Check if the space exist.
		map<string, array<set<Component*>, MAX_COMPONENTS> >::iterator mapIter = componentMap.find(gameObject.GetSpace());
		if (mapIter != componentMap.end()) {
			mapIter->second[id].insert(componentPtr); //Insert the component pointer.
		} else {
			array<set<Component*>, MAX_COMPONENTS> componentArray;
			componentArray[id].insert(componentPtr);
			componentMap.insert(std::pair<string, array<set<Component*>, MAX_COMPONENTS> >(gameObject.GetSpace(), componentArray));
		}

		return *componentPtr;
	}*/
	
	/*template <class Type>
	void RemoveComponent(Type& component) {
		Type* componentPtr = &component;
		ComponentTypeID id = GetComponentTypeID<Type>();
		componentMap.find(component.GetGameObject().GetSpace())->second[id].erase(componentPtr);
		delete componentPtr;
	}*/	

};

#endif
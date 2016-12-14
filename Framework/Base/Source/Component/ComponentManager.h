#ifndef COMPONENT_MANAGER_H
#define COMPONENT_MANAGER_H

#include "SingletonTemplate.h"
#include "../Others/IDGenerator.h"
#include <set>
#include <map>
#include <string>
#include <array>

//Temporary
#include "../Component/Physics/Transform.h"

using namespace std;

//Fuck me, so many templates. I hope this thing runs.

class GameObject;

//Welp, this looks complicated. I'm using std::array as a work around.
typedef map<string, array<set<Component*>, MAX_COMPONENTS> > ComponentMap;

class ComponentManager : public Singleton<ComponentManager> {

	friend class Singleton<ComponentManager>;

private:
	//Variable(s)	
	ComponentMap componentMap;
	set<Component*> addQueue[MAX_COMPONENTS];
	set<Component*> removeQueue[MAX_COMPONENTS];

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
		Type* componentPtr = new Type(gameObject, {});
		addQueue[IDGenerator::GetInstance().GetComponentTypeID<Type>()].insert(componentPtr);
		return *componentPtr;
	}

	void RemoveComponent(Component& component, ComponentTypeID id) {
		removeQueue[id].insert(&component);
	}

	template <class Type>
	set<Component*> GetComponents(const string& space) {
		if (componentMap.count(space) == 0) {
			array<set<Component*>, MAX_COMPONENTS> componentArray;
			componentMap.insert(std::pair<string, array<set<Component*>, MAX_COMPONENTS> >(space, componentArray));			
		}
		return componentMap.find(space)->second[IDGenerator::GetInstance().GetComponentTypeID<Type>()];
	}

};

#endif
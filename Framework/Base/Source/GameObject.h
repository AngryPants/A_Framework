#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include "ComponentTypeID.h"
#include <string>
#include <bitset>
#include <exception>
#include <set>
#include <map>
#include "ComponentManager.h"
#include "Script.h"

using namespace std;

typedef std::bitset<MAX_COMPONENTS> ComponentBitset;

class GameObject {

private:
	//Variable(s)
	string space;
	ComponentBitset componentBitset;
	Component* components[MAX_COMPONENTS];
	
public:
	//Variable(s)
	string name;
	Script* scripts[8];

	//Constructor(s) & Destructor
	GameObject(const string& space, const string& name = "GameObject") : space(space), name(name) {
		for (unsigned int i = 0; i < MAX_COMPONENTS; ++i) {
			this->components[i] = nullptr;
		}		
		for (unsigned int i = 0; i < sizeof(scripts)/sizeof(scripts[0]); ++i) {
			scripts[i] = nullptr;
		}
	}
	virtual ~GameObject() {
		for (unsigned int i = 0; i < MAX_COMPONENTS; ++i) {
			if (this->components[i] != nullptr) {
				ComponentManager::GetInstance().RemoveComponent(*components[i]);
			}
		}
		for (unsigned int i = 0; i < sizeof(scripts)/sizeof(scripts[0]); ++i) {
			if (scripts[i] != nullptr) {
				delete scripts[i];
			}
		}
	}

	//Name
	void SetName(const string& name) {
		this->name = name;
	}
	const string& GetName() const {
		return this->name;
	}
	const string& GetSpace() const {
		return this->space;
	}

	//Components
	template <class Type>
	bool HasComponent() const {
		return componentBitset[GetComponentTypeID<Type>()];
	}

	template <class Type>
	Type& AddComponent() {
		if (HasComponent<Type>()) {
			throw std::exception("This GameObject already has this component.");
		}

		ComponentTypeID id = GetComponentTypeID<Type>();
		componentBitset[id] = true;
		Component* componentPtr = &ComponentManager::GetInstance().CreateComponent<Type>(*this);
		components[id] = componentPtr;
		
		return *(static_cast<Type*>(componentPtr));
	}

	template <class Type>
	void RemoveComponent() {
		if (!HasComponent<Type>) {
			throw std::exception("This GameObject does not have this component.");
			return;
		}

		ComponentTypeID id = GetComponentTypeID<Type>();
		componentBitset[id] = 0;
		ComponentManager::GetInstance().RemoveComponent(*components[id]);
		components[id] = nullptr;
	}

	template <class Type>
	Type& GetComponent() {
		if (!HasComponent<Type>()) {
			throw std::exception("This GameObject does not have this component.");
		}

		Component* componentPtr = components[GetComponentTypeID<Type>()];
		return *(static_cast<Type*>(componentPtr));
	}
	
	const ComponentBitset& GetComponentBitset() const {
		return this->componentBitset;
	}

};

#endif
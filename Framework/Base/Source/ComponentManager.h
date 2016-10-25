#ifndef COMPONENT_MANAGER_H
#define COMPONENT_MANAGER_H

#include "ItemIndex.h"
#include "ComponentTypeID.h"
#include "Typedef.h"

class ComponentManager {

private:
	ItemIndex<Component*> components[MAX_COMPONENTS];

public:
	template <class Type>
	ComponentIndex CreateComponent(EntityIndex ownerID) {
		Component* componentPtr = new Type(ownerID);
		return components[GetComponentTypeID<Type>()].AddItem(componentPtr);
	}

	template <class Type>
	Type& GetComponent(ComponentIndex index) {
		return *(components[GetComponentTypeID<Type>()].GetItem(index));
	}

	template <class Type>
	void RemoveComponent(ComponentIndex index) {
		components[GetComponentTypeID<Type>()].DeleteItem(index);
	}

	template <class Type>
	unsigned int GetNumComponents() const {
		return components[GetComponentTypeID<Type>()].GetNumValid();
	}

	template <class Type>
	const set<ComponentIndex>& GetValidIndexes() const {
		return components[GetComponentTypeID<Type>()].GetValidIndexes();
	}

};

#endif
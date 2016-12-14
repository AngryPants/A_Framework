#ifndef COMPONENT_ID_H
#define COMPONENT_ID_H

#include "SingletonTemplate.h"
#include "../Component/Component.h"
#include "PassKey.h"
#include <type_traits>
#include <exception>
#include <string>
#include <iostream>
#include <set>

using namespace std;

typedef int ComponentTypeID; //The ID of the Component TYPE.
typedef long int GameObjectID; //Unique ID for every GameObject.

static const ComponentTypeID INVALID_COMPONENT_TYPE_ID = -1;
static const GameObjectID INVALID_GAMEOBJECT_ID = -1;

//Handles ID Generation
class IDGenerator : public Singleton<IDGenerator> {

	friend class Singleton<IDGenerator>;

private:
	//Variable(s)
	set<GameObjectID> usedGameObjectIDs;
	set<GameObjectID> freeGameObjectIDs;

	//Constructor(s) & Destructor
	IDGenerator() {}
	virtual ~IDGenerator() {}

	//Private Function(s)
	ComponentTypeID GenerateComponentTypeID() {
		static ComponentTypeID currentID = 0;
		return currentID++;
	}

public:
	//Calling this function will return an ID for each component type.
	template <class Type>
	ComponentTypeID GetComponentTypeID() {
		if (!std::is_base_of<Component, Type>::value) {
			string errorMessage = "Cannot use GetComponentTypeID() on non-components.";
			cout << errorMessage << endl;
	        throw std::exception(errorMessage.c_str());
	    }
	
	    static ComponentTypeID componentID = GenerateComponentTypeID();
		return componentID;
	}

	GameObjectID GenerateGameObjectID(PassKey<GameObject> _key) {
		GameObjectID id = usedGameObjectIDs.size();
		if (freeGameObjectIDs.empty() == false) {
			id = *freeGameObjectIDs.begin();
			freeGameObjectIDs.erase(freeGameObjectIDs.begin());
		}
		usedGameObjectIDs.insert(id);
		return id;
	}

	void ReturnGameObjectID(const GameObjectID _id, PassKey<GameObject> _key) {
		//Find the ID.
		set<GameObjectID>::iterator setIter = usedGameObjectIDs.find(_id);
		//Check if the ID is valid.
		if (setIter == usedGameObjectIDs.end()) {
			string errorMessage = "Error! ReturnGameObjectID() failed as ID is not valid.";
			cout << errorMessage << endl;
			throw std::exception(errorMessage.c_str());
		}
		//Do the necessary actions.
		usedGameObjectIDs.erase(setIter);
		freeGameObjectIDs.insert(_id);
		//If there are no more usedGameObjectIDs, it means that there are no more gameObjects.
		if (usedGameObjectIDs.empty()) {
			freeGameObjectIDs.clear();
		}
		//Remove the ids we don't need.
		while (!freeGameObjectIDs.empty() && !usedGameObjectIDs.empty() && *freeGameObjectIDs.rbegin() > *usedGameObjectIDs.rbegin()) {
			freeGameObjectIDs.erase(--freeGameObjectIDs.end());
		}
	}

};



#endif
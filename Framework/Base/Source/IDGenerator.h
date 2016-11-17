#ifndef COMPONENT_ID_H
#define COMPONENT_ID_H

#include "SingletonTemplate.h"
#include "Component.h"
#include <type_traits>
#include <exception>
#include "Error.h"
#include <string>

typedef unsigned int ComponentTypeID; //The ID of the component TYPE.

static const unsigned int MAX_COMPONENTS = 64;

class IDGenerator : public Singleton<IDGenerator> {

	friend class Singleton<IDGenerator>;

private:
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
	        Error::ThrowException("Cannot use GetComponentTypeID() on non-components.");
	    }
	
	    static ComponentTypeID componentID = GenerateComponentTypeID();
		return componentID;
	}

};



#endif
#ifndef COMPONENT_H
#define COMPONENT_H

#include "Typedef.h"
#include <string>

using std::string;

class GameObject;

//NOTE: Any subclasses MUST have a constructor that only takes in an GameObject reference;

class Component {
	
private:
	//Variable(s)
	GameObject* gameObject; //The GameObject that owns this Component.
	string space;

public:
	//Variable(s)
	const string name;
	bool isActive;
	
	//Destructor
	virtual ~Component();
	
	//Interface Function(s)
	GameObject& GetGameObject() {
		return *gameObject;
	}

	const string& GetSpace() const {
		return space;
	}

protected:
	//Constructor(s)
	Component(const string& name, GameObject& gameObject);

};

#endif
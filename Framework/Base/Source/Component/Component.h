#ifndef COMPONENT_H
#define COMPONENT_H

#include <string>
#include <iostream>

using std::string;
using std::cout;
using std::endl;

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
	GameObject& GetGameObject() const {
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
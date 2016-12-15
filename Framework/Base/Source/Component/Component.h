#ifndef COMPONENT_H
#define COMPONENT_H

#include <string>
#include <iostream>
#include "PassKey.h"

using std::string;
using std::cout;
using std::endl;

class GameObject;
class ComponentManager;

static const unsigned int MAX_COMPONENTS = 64;

//NOTE: Any subclasses MUST have a constructor that only takes in an GameObject reference;
class Component {
	
private:
	//Variable(s)
	GameObject* gameObject; //The GameObject that owns this Component.
	string space;	

public:
	//Variable(s)
	const string name;
		
	//Destructor
	virtual ~Component();
	
	//Interface Function(s)
	GameObject& GetGameObject();
	GameObject& GetGameObject() const;
	const string& GetSpace() const;
	bool IsActive() const;
	virtual void SetActive();
	virtual void SetInActive();
		
protected:
	//Variable(s)
	bool isActive;	

	//Constructor(s)
	Component(const string& _name, GameObject& _gameObject, PassKey<ComponentManager> _key);

};

#endif
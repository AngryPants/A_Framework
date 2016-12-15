#include "Component.h"
#include "../GameObject/GameObject.h"
#include "ComponentManager.h"

//Constructor(s) & Destructor
Component::Component(const string& _name, GameObject& _gameObject, PassKey<ComponentManager> _key) : name(_name) {
	this->gameObject = &_gameObject;
	this->space = _gameObject.GetSpace();
	this->isActive = true;
}

Component::~Component() {
}

//Interface Function(s)
GameObject& Component::GetGameObject() {
	return *gameObject;
}
GameObject& Component::GetGameObject() const {
	return *gameObject;
}

const string& Component::GetSpace() const {
	return space;
}

bool Component::IsActive() const {
	return isActive;
}
void Component::SetActive() {
	this->isActive = true;
}

void Component::SetInActive() {
	this->isActive = false;
}
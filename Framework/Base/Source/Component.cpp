#include "Component.h"
#include "GameObject.h"

Component::Component(const string& name, GameObject& gameObject) : name(name) {
	this->gameObject = &gameObject;
	this->space = gameObject.GetSpace();
	this->isActive = true;
}

Component::~Component() {
}